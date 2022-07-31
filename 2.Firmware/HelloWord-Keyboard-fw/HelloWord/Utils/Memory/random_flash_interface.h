#ifndef FlashStorage_STM32_h
#define FlashStorage_STM32_h

#include "random_flash_utils.h"


class EEPROM
{
public:
    EEPROM()
    = default;


    uint8_t Read(int _address)
    {
        if (!isInitialized)
            init();

        return EEPROMReadBufferedByte(_address);
    }


    void Update(int _address, uint8_t _value)
    {
        if (!isInitialized)
            init();

        if (EEPROMReadBufferedByte(_address) != _value)
        {
            dirtyBuffer = true;
            EEPROMWriteBufferedByte(_address, _value);
        }
    }


    void Write(int _address, uint8_t _value)
    {
        Update(_address, _value);
    }


    template<typename T>
    T &Pull(int _offset, T &_t)
    {
        // Copy the data from the flash to the buffer if not yet
        if (!isInitialized)
            init();

        uint16_t offset = _offset;
        auto* _pointer = (uint8_t*) &_t;

        for (uint16_t count = sizeof(T); count; --count, ++offset)
        {
            *_pointer++ = EEPROMReadBufferedByte(offset);
        }

        return _t;
    }


    template<typename T>
    const T &Push(int _idx, const T &_t)
    {
        // Copy the data from the flash to the buffer if not yet
        if (!isInitialized) init();

        uint16_t offset = _idx;

        const auto* _pointer = (const uint8_t*) &_t;

        for (uint16_t count = sizeof(T); count; --count, ++offset)
        {
            EEPROMWriteBufferedByte(offset, *_pointer++);
        }

        if (commitASAP)
        {
            // Save the data from the buffer to the flash right away
            EEPROMBufferFlush();

            dirtyBuffer = false;
            isValid = true;
        } else
        {
            // Delay saving the data from the buffer to the flash. Just flag and wait for commit() later
            dirtyBuffer = true;
        }

        return _t;
    }


    void Commit()
    {
        if (!isInitialized)
            init();

        if (dirtyBuffer)
        {
            // Save the data from the buffer to the flash
            EEPROMBufferFlush();

            dirtyBuffer = false;
            isValid = true;
        }
    }


    static uint16_t TotalSize()
    {
        return EEPROM_SIZE + 1;
    }


    void SetCommitASAP(bool value = true)
    {
        commitASAP = value;
    }


    bool isValid = true;


private:
    void init()
    {
        // Copy the data from the flash to the buffer
        EEPROMFillBuffer();
        isInitialized = true;
    }


    bool isInitialized = false;
    bool dirtyBuffer = false;
    bool commitASAP = true;
};


#endif
