#include "st_hardware.h"
#include "stm32f103xb.h"

uint64_t GetSerialNumber()
{
    uint32_t uuid0 = *(uint32_t * )(UID_BASE + 0);
    uint32_t uuid1 = *(uint32_t * )(UID_BASE + 4);
    uint32_t uuid2 = *(uint32_t * )(UID_BASE + 8);
    uint32_t uuid_mixed_part = uuid0 + uuid2;
    uint64_t serialNumber = ((uint64_t) uuid_mixed_part << 16) | (uint64_t)(uuid1 >> 16);

    return serialNumber;
}