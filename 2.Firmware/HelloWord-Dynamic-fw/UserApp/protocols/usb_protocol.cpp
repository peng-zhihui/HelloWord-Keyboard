#include "common_inc.h"
#include "eink_290_bw.h"

/*----------------- 1.Add Your Extern Variables Here (Optional) ------------------*/

class HelperFunctions
{
public:
    /*--------------- 2.Add Your Helper Functions Helper Here (optional) ----------------*/
    int32_t TestFunction(int32_t delta)
    {
        static int cnt = 0;
        printf("%ld,%d\n", delta, cnt);
        return cnt += delta;
    }

    void SaveConfigurationHelper()
    {}

    void EraseConfigurationHelper()
    {}

    float GetTemperatureHelper()
    { return AdcGetChipTemperature(); }

    void SystemResetHelper()
    { NVIC_SystemReset(); }

} staticFunctions;


// Define options that intractable with "reftool".
static inline auto MakeObjTree()
{
    /*--------------- 3.Add Your Protocol Variables & Functions Here ----------------*/
    return make_protocol_member_list(
        // Add Read-Only Variables
        make_protocol_ro_property("serial_number", &serialNumber),

        // Add Hierarchy Menus
        make_protocol_object("config",
            // Add Read-Write Variables
                             make_protocol_property("can_node_id", &boardConfig.canNodeId)
        ),

        // Add Functions (must be Member-Functions, so we add a staticFunctions as Helper-Object)
        make_protocol_function("test_function", staticFunctions, &HelperFunctions::TestFunction, "delta"),
        make_protocol_function("save_configuration", staticFunctions,
                               &HelperFunctions::SaveConfigurationHelper),
        make_protocol_function("erase_configuration", staticFunctions,
                               &HelperFunctions::EraseConfigurationHelper),
        make_protocol_function("get_temperature", staticFunctions, &HelperFunctions::GetTemperatureHelper),
        make_protocol_function("reboot", staticFunctions, &HelperFunctions::SystemResetHelper)
    );
}


extern Eink290BW eink;
/*--------------- 4.Handle USB-Bulk Transmit Event Here ----------------*/
uint8_t* OnBulkPacket(const uint8_t* _buffer, size_t _bufferLength,
                      uint32_t _packetWriteAddressOffset, // For host write it indicates current packet offset
                      uint32_t _readDataLength) // For host read it indicates total data length that host required
{

    if (_bufferLength == 3)
    {
        eink.DrawBitmap(Eink290BW::buffer);
        eink.Update();
    }

    if (_readDataLength > 0) // Host Read
    {
        return nullptr;
    } else // Host Write
    {
        memcpy(Eink290BW::buffer + _packetWriteAddressOffset, _buffer, _bufferLength);
        return nullptr;
    }
}


COMMIT_PROTOCOL
