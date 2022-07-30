#ifndef MAGNETICSENSORSPI_LIB_H
#define MAGNETICSENSORSPI_LIB_H

#include "Ctrl/Sensor/Encoder/encoder_base.h"
#include "Ctrl/Motor/math_utils.h"


class EncoderAS5047Base : public EncoderBase
{
public:
    const static uint8_t RESOLUTION_BITS = 14;
    const static uint16_t COUNT_PER_ROTATE = 16384;
    const static uint16_t ANGLE_REG = 0x3FFF;
    const static uint8_t COMMAND_PARITY_BIT = 15;
    const static uint8_t COMMAND_RW_BIT = 14;
    const static uint8_t DATA_START_BIT = 13;
    const static uint16_t DATA_MASK = 0xFFFF >> (16 - RESOLUTION_BITS);


    EncoderAS5047Base()
    = default;

    void Init() override;


private:
    float GetRawAngle() override;
    uint16_t GetRawData();
    uint8_t SpiCalcEvenParity(uint16_t value);


    uint16_t command;
    uint16_t registerVal;

    /***** Platform Specified Implements *****/
    virtual void SpiInit() = 0;
    virtual uint16_t SpiTransmitAndRead16Bits(uint16_t _dataTx) = 0;
};


#endif
