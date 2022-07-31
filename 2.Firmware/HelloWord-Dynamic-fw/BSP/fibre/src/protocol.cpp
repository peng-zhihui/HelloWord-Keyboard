#include <memory>
#include "../inc/protocol.hpp"
#include "../inc/crc.hpp"
#include "usbd_cdc.h"


Endpoint** endpoint_list_ = nullptr; // initialized by calling fibre_publish
size_t n_endpoints_ = 0; // initialized by calling fibre_publish
uint16_t json_crc_; // initialized by calling fibre_publish
JSONDescriptorEndpoint json_file_endpoint_ = JSONDescriptorEndpoint();
EndpointProvider* application_endpoints_;


static void hexdump(const uint8_t* buf, size_t len);
static inline int write_string(const char* str, StreamSink* output);


int StreamToPacketSegmenter::process_bytes(uint8_t* buffer, size_t length, size_t* processed_bytes)
{
    int result = 0;

    while (length--)
    {
        if (header_index_ < sizeof(header_buffer_))
        {
            // Process header byte
            header_buffer_[header_index_++] = *buffer;
            if (header_index_ == 1 && header_buffer_[0] != CANONICAL_PREFIX)
            {
                header_index_ = 0;
            } else if (header_index_ == 2 && (header_buffer_[1] & 0x80))
            {
                header_index_ = 0; // TODO: support packets larger than 128 bytes
            } else if (header_index_ == 3 &&
                       calc_crc8<CANONICAL_CRC8_POLYNOMIAL>(CANONICAL_CRC8_INIT, header_buffer_, 3))
            {
                header_index_ = 0;
            } else if (header_index_ == 3)
            {
                packet_length_ = header_buffer_[1] + 2;
            }
        } else if (packet_index_ < sizeof(packet_buffer_))
        {
            // Process payload byte
            packet_buffer_[packet_index_++] = *buffer;
        }

        // If both header and packet are fully received, hand it on to the packet processor
        if (header_index_ == 3 && packet_index_ == packet_length_)
        {
            if (calc_crc16<CANONICAL_CRC16_POLYNOMIAL>(CANONICAL_CRC16_INIT, packet_buffer_,
                                                       packet_length_) == 0)
            {
                result |= output_.process_packet(packet_buffer_, packet_length_ - 2);
            }
            header_index_ = packet_index_ = packet_length_ = 0;
        }
        buffer++;
        if (processed_bytes)
            (*processed_bytes)++;
    }

    return result;
}

int StreamBasedPacketSink::process_packet(uint8_t* buffer, size_t length)
{
    // TODO: support buffer size >= 128
    if (length >= 128)
        return -1;

    LOG_FIBRE("send header\r\n");
    uint8_t header[] = {
        CANONICAL_PREFIX,
        static_cast<uint8_t>(length),
        0
    };
    header[2] = calc_crc8<CANONICAL_CRC8_POLYNOMIAL>(CANONICAL_CRC8_INIT, header, 2);

    if (output_.process_bytes(header, sizeof(header), nullptr))
        return -1;

    if (output_.process_bytes(buffer, length, nullptr))
        return -1;

    LOG_FIBRE("send crc16\r\n");
    uint16_t crc16 = calc_crc16<CANONICAL_CRC16_POLYNOMIAL>(CANONICAL_CRC16_INIT, buffer, length);
    uint8_t crc16_buffer[] = {
        (uint8_t) ((crc16 >> 8) & 0xff),
        (uint8_t) ((crc16 >> 0) & 0xff)
    };
    if (output_.process_bytes(crc16_buffer, 2, nullptr))
        return -1;
    LOG_FIBRE("sent!\r\n");
    return 0;
}


void JSONDescriptorEndpoint::write_json(size_t id, StreamSink* output)
{
    write_string("{\"name\":\"\",", output);

    // write endpoint ID
    write_string("\"id\":", output);
    char id_buf[10];
    snprintf(id_buf, sizeof(id_buf), "%u", (unsigned) id); // TODO: get rid of printf
    write_string(id_buf, output);

    write_string(",\"type\":\"json\",\"access\":\"r\"}", output);
}

void JSONDescriptorEndpoint::register_endpoints(Endpoint** list, size_t id, size_t length)
{
    if (id < length)
        list[id] = this;
}

// Returns part of the JSON interface definition.
void JSONDescriptorEndpoint::handle(const uint8_t* input, size_t input_length, StreamSink* output)
{
    // The request must contain a 32 bit integer to specify an offset
    if (input_length < 4)
        return;
    uint32_t offset = 0;
    read_le<uint32_t>(&offset, input);
    NullStreamSink output_with_offset = NullStreamSink(offset, *output);

    size_t id = 0;
    write_string("[", &output_with_offset);
    json_file_endpoint_.write_json(id, &output_with_offset);
    id += decltype(json_file_endpoint_)::endpoint_count;
    write_string(",", &output_with_offset);
    application_endpoints_->write_json(id, &output_with_offset);
    write_string("]", &output_with_offset);
}

int BidirectionalPacketBasedChannel::process_packet(uint8_t* buffer, size_t length)
{
    if (length < 4) return -1;

    auto packageCount = read_le<uint16_t>(&buffer, &length);

    if (packageCount & 0x8000) // BULK Packet
    {
        bool needResponse = packageCount & 0x4000;
        if (needResponse)
        {
            auto requestedLength = read_le<uint32_t>(&buffer, &length);

            uint8_t* sendBufferPtr = OnBulkPacket(buffer, length, 0, requestedLength);

            uint32_t n = requestedLength / CDC_DATA_MAX_PACKET_SIZE;
            uint32_t addressOffset = 0;
            while (n--)
            {
                int ret;
                do
                {
                    ret = output_.process_packet(sendBufferPtr + addressOffset,
                                                 CDC_DATA_MAX_PACKET_SIZE);
                } while (ret != 0);

                addressOffset += CDC_DATA_MAX_PACKET_SIZE;
            }

            uint32_t lastPacketLength = requestedLength % CDC_DATA_MAX_PACKET_SIZE;
            if (lastPacketLength != 0)
            {
                int ret;
                do
                {
                    ret = output_.process_packet(sendBufferPtr + addressOffset, lastPacketLength);
                } while (ret != 0);
            }
        } else
        {
            uint16_t addressOffsetCount = packageCount & 0x3FFF;
            OnBulkPacket(buffer, length,
                         addressOffsetCount * (CDC_DATA_MAX_PACKET_SIZE-2),
                         0);
        }
    } else // RPC Packet
    {
        // standard packet
        auto endpoint_id = read_le<uint16_t>(&buffer, &length);
        endpoint_id &= 0x7fff;

        if (endpoint_id >= n_endpoints_)
            return -1;

        Endpoint* endpoint = endpoint_list_[endpoint_id];
        if (!endpoint)
        {
            LOG_FIBRE("critical: no endpoint at %d", endpoint_id);
            return -1;
        }

        // Verify packet trailer. The expected trailer value depends on the selected endpoint.
        // For endpoint 0 this is just the protocol version, for all other endpoints it's a
        // CRC over the entire JSON descriptor tree (this may change in future versions).
        uint16_t expected_trailer = endpoint_id ? json_crc_ : PROTOCOL_VERSION;
        uint16_t actual_trailer = buffer[length - 2] | (buffer[length - 1] << 8);
        if (expected_trailer != actual_trailer)
        {
        }

        // TODO: if more bytes than the MTU were requested, should we abort or just return as much as possible?

        auto expected_response_length = read_le<uint16_t>(&buffer, &length);

        // Limit response length according to our local TX buffer size
        if (expected_response_length > sizeof(tx_buf_) - 2)
            expected_response_length = sizeof(tx_buf_) - 2;

        MemoryStreamSink output(tx_buf_ + 2, expected_response_length);
        endpoint->handle(buffer, length - 2, &output);

        // Send response
        size_t actual_response_length = expected_response_length - output.get_free_space() + 2;
        write_le<uint16_t>(packageCount | 0x8000, tx_buf_);

        output_.process_packet(tx_buf_, actual_response_length);
    }

    return 0;
}

bool is_endpoint_ref_valid(endpoint_ref_t endpoint_ref)
{
    return (endpoint_ref.json_crc == json_crc_)
           && (endpoint_ref.endpoint_id < n_endpoints_);
}

Endpoint* get_endpoint(endpoint_ref_t endpoint_ref)
{
    if (is_endpoint_ref_valid(endpoint_ref))
        return endpoint_list_[endpoint_ref.endpoint_id];
    else
        return nullptr;
}
