#ifndef CTRL_STEP_FW_AS5047_M0_STM32_H
#define CTRL_STEP_FW_AS5047_M0_STM32_H

#include "Ctrl/Sensor/Encoder/Instances/encoder_as5047.h"


class Encoder : public EncoderAS5047Base
{
public:
    explicit Encoder(SPI_HandleTypeDef* _spi) : EncoderAS5047Base()
    {
        spiHandle = _spi;
    }

    SPI_HandleTypeDef* spiHandle = nullptr;


private:
    void SpiInit() override;

    uint16_t SpiTransmitAndRead16Bits(uint16_t _data) override;
};

#endif
