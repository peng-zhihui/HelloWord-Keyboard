#ifndef REF_STM32F4_COMMON_INC_H
#define REF_STM32F4_COMMON_INC_H

#define CONFIG_FW_VERSION 1.0

/*---------------------------- C Scope ---------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "freertos_inc.h"
#include "adc.h"
#include "tim.h"
#include "i2c.h"
#include "spi.h"


void Main(void);

extern uint64_t serialNumber;
extern char serialNumberStr[13];


#ifdef __cplusplus
}

/*---------------------------- C++ Scope ---------------------------*/
#include "Platform/Utils/Memory/random_flash_utils.h"
#include "Platform/Utils/time_utils.h"
#include "Platform/Utils/timer.hpp"
#include "Platform/Utils/st_hardware.h"
#include "Platform/Communication/communication.hpp"

// Universal Board Configs definition
struct BoardConfig_t
{
    uint8_t canNodeId = 1;
    bool userConfigLoaded = false;
};
extern struct BoardConfig_t boardConfig;


#endif
#endif //REF_STM32F4_COMMON_INC_H
