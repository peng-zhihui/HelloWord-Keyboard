#include "encoder_as5047.h"


void EncoderAS5047Base::Init()
{
    SpiInit();
    VarInit();

    command = ANGLE_REG | (1 << COMMAND_RW_BIT);
    // Add a parity bit on the MSB
    command |= ((uint16_t) SpiCalcEvenParity(command) << COMMAND_PARITY_BIT);
}


float EncoderAS5047Base::GetRawAngle()
{
    return ((float) GetRawData() / (float) COUNT_PER_ROTATE) * _2PI;
}


uint16_t EncoderAS5047Base::GetRawData()
{
    SpiTransmitAndRead16Bits(command);

    // 350ns is the required time for AMS sensors, 80ns for MA730, MA702
    // Need to be tuned for different CPU
    uint8_t _us = 10;
    while (_us--) __NOP();

    // This should shift data to the rightmost bits of the word
    registerVal = SpiTransmitAndRead16Bits(0x00) >> (1 + DATA_START_BIT - RESOLUTION_BITS);

    // Return the data, stripping the non data (e.g. parity) bits
    return registerVal & DATA_MASK;
}


uint8_t EncoderAS5047Base::SpiCalcEvenParity(uint16_t value)
{
    uint8_t cnt = 0;
    uint8_t i;

    for (i = 0; i < 16; i++)
    {
        if (value & 0x1)
            cnt++;

        value >>= 1;
    }

    return cnt & 0x1;
}
