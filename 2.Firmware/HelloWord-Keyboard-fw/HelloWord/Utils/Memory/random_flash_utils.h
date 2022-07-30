#ifndef __STM32_EEPROM_H
#define __STM32_EEPROM_H

#ifdef __cplusplus
extern "C" {
#include <stm32f103xb.h>
#include <stm32f1xx_hal.h>
#endif

#define EEPROM_SIZE  FLASH_PAGE_SIZE // 1K Byte


uint8_t EEPROMReadByte(uint32_t _pos);
void EEPROMWriteByte(uint32_t _pos, uint8_t _value);

void EEPROMFillBuffer();
void EEPROMBufferFlush();
uint8_t EEPROMReadBufferedByte(uint32_t _pos);
void EEPROMWriteBufferedByte(uint32_t _pos, uint8_t _value);


#ifdef __cplusplus
}
#endif
#endif
