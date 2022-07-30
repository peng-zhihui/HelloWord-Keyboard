#include <cstring>
#include "hw_keyboard.h"


inline void DelayUs(uint32_t _us)
{
    for (int i = 0; i < _us; i++)
        for (int j = 0; j < 8; j++)  // ToDo: tune this for different chips
            __NOP();
}


uint8_t* HWKeyboard::ScanKeyStates()
{
    memset(spiBuffer, 0xFF, IO_NUMBER / 8 + 1);
    PL_GPIO_Port->BSRR = PL_Pin; // Latch

    spiHandle->pRxBuffPtr = (uint8_t*) spiBuffer;
    spiHandle->RxXferCount = IO_NUMBER / 8 + 1;
    __HAL_SPI_ENABLE(spiHandle);
    while (spiHandle->RxXferCount > 0U)
    {
        if (__HAL_SPI_GET_FLAG(spiHandle, SPI_FLAG_RXNE))
        {
            /* read the received data */
            (*(uint8_t*) spiHandle->pRxBuffPtr) = *(__IO uint8_t*) &spiHandle->Instance->DR;
            spiHandle->pRxBuffPtr += sizeof(uint8_t);
            spiHandle->RxXferCount--;
        }
    }
    __HAL_SPI_DISABLE(spiHandle);

    PL_GPIO_Port->BRR = PL_Pin; // Sample
    return scanBuffer;
}


void HWKeyboard::ApplyDebounceFilter(uint32_t _filterTimeUs)
{
    memcpy(debounceBuffer, spiBuffer, IO_NUMBER / 8 + 1);

    DelayUs(_filterTimeUs);
    ScanKeyStates();

    uint8_t mask;
    for (int i = 0; i < IO_NUMBER / 8 + 1; i++)
    {
        mask = debounceBuffer[i] ^ spiBuffer[i];
        spiBuffer[i] |= mask;
    }
}


uint8_t* HWKeyboard::Remap(uint8_t _layer)
{
    int16_t index, bitIndex;

    memset(remapBuffer, 0, IO_NUMBER / 8);
    for (int16_t i = 0; i < IO_NUMBER / 8; i++)
    {
        for (int16_t j = 0; j < 8; j++)
        {
            index = (int16_t) (keyMap[0][i * 8 + j] / 8);
            bitIndex = (int16_t) (keyMap[0][i * 8 + j] % 8);
            if (scanBuffer[index] & (0x80 >> bitIndex))
                remapBuffer[i] |= 0x80 >> j;
        }
        remapBuffer[i] = ~remapBuffer[i];
    }

    memset(hidBuffer, 0, KEY_REPORT_SIZE);
    for (int i = 0; i < IO_NUMBER / 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            index = (int16_t) (keyMap[_layer][i * 8 + j] / 8 + 1); // +1 for modifier
            bitIndex = (int16_t) (keyMap[_layer][i * 8 + j] % 8);
            if (bitIndex < 0)
            {
                index -= 1;
                bitIndex += 8;
            } else if (index > 100)
                continue;

            if (remapBuffer[i] & (0x80 >> j))
                hidBuffer[index + 1] |= 1 << (bitIndex); // +1 for Report-ID
        }
    }

    return hidBuffer;
}


bool HWKeyboard::FnPressed()
{
    return remapBuffer[9] & 0x02;
}


void HWKeyboard::SetRgbBuffer(uint8_t _keyId, HWKeyboard::Color_t _color)
{
    for (int i = 0; i < 8; i++)
    {
        rgbBuffer[_keyId][0][i] = (_color.g >> brightnessPrediv) & (0x80 >> i) ? WS_HIGH : WS_LOW;
        rgbBuffer[_keyId][1][i] = (_color.r >> brightnessPrediv) & (0x80 >> i) ? WS_HIGH : WS_LOW;
        rgbBuffer[_keyId][2][i] = (_color.b >> brightnessPrediv) & (0x80 >> i) ? WS_HIGH : WS_LOW;
    }
}


void HWKeyboard::SyncLights()
{
    while (isRgbTxBusy);
    isRgbTxBusy = true;
    HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*) rgbBuffer, LED_NUMBER * 3 * 8);
    while (isRgbTxBusy);
    isRgbTxBusy = true;
    HAL_SPI_Transmit_DMA(&hspi2, wsCommit, 64);
}


uint8_t* HWKeyboard::GetHidReportBuffer(uint8_t _reportId)
{
    switch (_reportId)
    {
        case 1:
            hidBuffer[0] = 1;
            return hidBuffer;
        case 2:
            hidBuffer[KEY_REPORT_SIZE] = 2;
            return hidBuffer + KEY_REPORT_SIZE;
        default:
            return hidBuffer;
    }
}
