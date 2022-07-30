#ifndef HELLOWORD_DYNAMIC_FW_RGB_LIGHT_H
#define HELLOWORD_DYNAMIC_FW_RGB_LIGHT_H

#include "spi.h"

class RGB
{
public:
    explicit RGB(SPI_HandleTypeDef* _spi):
        spiHandle(_spi)
    {
    }

    struct Color_t
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };
    enum SpiWs2812Byte_t : uint8_t
    {
        WS_HIGH = 0xFE,
        WS_LOW = 0xE0
    };
    static const uint8_t LED_NUMBER = 4;
    volatile bool isRgbTxBusy;

    void SetRgbBuffer(uint8_t _keyId, Color_t _color);
    void SyncLights();

private:
    SPI_HandleTypeDef* spiHandle;
    uint8_t rgbBuffer[LED_NUMBER][3][8]{};
    uint8_t wsCommit[64] = {0};
    uint8_t brightnessPrediv = 2; // 1/4

};

#endif //HELLOWORD_DYNAMIC_FW_RGB_LIGHT_H
