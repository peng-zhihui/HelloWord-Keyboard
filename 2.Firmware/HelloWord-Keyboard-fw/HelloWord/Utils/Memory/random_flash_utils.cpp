#ifndef __STM32_EEPROM_HPP
#define __STM32_EEPROM_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include <cstring>
#include "random_flash_utils.h"


#define FLASH_BANK_NUMBER       FLASH_BANK_1
#define FLASH_END               FLASH_BANK1_END
#define FLASH_BASE_ADDRESS      ((uint32_t)((FLASH_END + 1) - EEPROM_SIZE))


static uint8_t eepromBuffer[EEPROM_SIZE] __attribute__((aligned(8))) = {0};


static inline uint32_t GetFlashEndAddress()
{
    uint32_t size;
    switch ((*((uint16_t*) FLASH_SIZE_DATA_REGISTER)))
    {
        case 0x200U:
            size = 0x0807FFFFU;
            break;
        case 0x100U:
            size = 0x0803FFFFU;
            break;
        case 0x80U:
            size = 0x0801FFFFU;
            break;
        case 0x40U:
            size = 0x0800FFFFU;
            break;
        case 0x20U:
            size = 0x08007FFFU;
            break;
        default:
            size = 0x08003FFFU;
            break;
    }
    return size;
}


uint8_t EEPROMReadByte(const uint32_t _pos)
{
    EEPROMFillBuffer();
    return EEPROMReadBufferedByte(_pos);
}


void EEPROMWriteByte(uint32_t _pos, uint8_t _value)
{
    EEPROMWriteBufferedByte(_pos, _value);
    EEPROMBufferFlush();
}


uint8_t EEPROMReadBufferedByte(const uint32_t _pos)
{
    return eepromBuffer[_pos];
}


void EEPROMWriteBufferedByte(uint32_t _pos, uint8_t _value)
{
    eepromBuffer[_pos] = _value;
}


void EEPROMFillBuffer(void)
{
    memcpy(eepromBuffer, (uint8_t*) (FLASH_BASE_ADDRESS), EEPROM_SIZE);
}


void EEPROMBufferFlush(void)
{
    FLASH_EraseInitTypeDef eraseInitStruct;
    uint32_t offset = 0;
    uint32_t address = FLASH_BASE_ADDRESS;
    uint32_t address_end = FLASH_BASE_ADDRESS + EEPROM_SIZE - 1;
    uint32_t pageError = 0;
    uint64_t data = 0;

    /* ERASING page */
    eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInitStruct.Banks = FLASH_BANK_NUMBER;
    eraseInitStruct.PageAddress = FLASH_BASE_ADDRESS;
    eraseInitStruct.NbPages = 1;

    if (HAL_FLASH_Unlock() == HAL_OK)
    {
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR);

        if (HAL_FLASHEx_Erase(&eraseInitStruct, &pageError) == HAL_OK)
        {
            while (address <= address_end)
            {
                data = *((uint64_t*) ((uint8_t*) eepromBuffer + offset));

                if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, data) == HAL_OK)
                {
                    address += 8;
                    offset += 8;
                } else
                    address = address_end + 1;
            }
        }
        HAL_FLASH_Lock();
    }
}


#ifdef __cplusplus
}
#endif
#endif

