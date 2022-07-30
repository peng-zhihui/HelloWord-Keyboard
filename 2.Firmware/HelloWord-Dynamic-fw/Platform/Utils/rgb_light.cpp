#include "rgb_light.h"


void RGB::SetRgbBuffer(uint8_t _keyId, RGB::Color_t _color)
{
    for (int i = 0; i < 8; i++)
    {
        rgbBuffer[_keyId][0][i] = (_color.g >> brightnessPrediv) & (0x80 >> i) ? WS_HIGH : WS_LOW;
        rgbBuffer[_keyId][1][i] = (_color.r >> brightnessPrediv) & (0x80 >> i) ? WS_HIGH : WS_LOW;
        rgbBuffer[_keyId][2][i] = (_color.b >> brightnessPrediv) & (0x80 >> i) ? WS_HIGH : WS_LOW;
    }
}


void RGB::SyncLights()
{
    while (isRgbTxBusy);
    isRgbTxBusy = true;
    HAL_SPI_Transmit_DMA(&hspi3, (uint8_t*) rgbBuffer, LED_NUMBER * 3 * 8);
    while (isRgbTxBusy);
    isRgbTxBusy = true;
    HAL_SPI_Transmit_DMA(&hspi3, wsCommit, 64);
}