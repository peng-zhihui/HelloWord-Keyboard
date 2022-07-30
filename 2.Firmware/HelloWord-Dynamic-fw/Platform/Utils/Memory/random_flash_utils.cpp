#ifndef __STM32_EEPROM_HPP
#define __STM32_EEPROM_HPP

#include <cstdint>
#include <cstring>
#include "random_flash_utils.h"


#define FLASH_BASE_ADDRESS      ((uint32_t)((FLASH_END + 1) - EEPROM_SIZE))
#define FLASH_DATA_SECTOR   ((uint32_t)(FLASH_SECTOR_TOTAL - 1))


static uint8_t eepromBuffer[EEPROM_SIZE] __attribute__((aligned(8))) = {0};


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
    uint32_t addressEnd = FLASH_BASE_ADDRESS + EEPROM_SIZE - 1;
    uint32_t sectorError = 0;
    uint64_t data = 0;

    /* ERASING page */
    eraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    eraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    eraseInitStruct.Sector = FLASH_DATA_SECTOR;
    eraseInitStruct.NbSectors = 1;

    HAL_FLASH_Unlock();

    if (HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError) == HAL_OK)
    {
        while (address <= addressEnd)
        {
            memcpy(&data, eepromBuffer + offset, sizeof(uint32_t));
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data) == HAL_OK)
            {
                address += 4;
                offset += 4;
            } else
            {
                address = addressEnd + 1;
            }
        }
    }
    HAL_FLASH_Lock();
}


#endif

