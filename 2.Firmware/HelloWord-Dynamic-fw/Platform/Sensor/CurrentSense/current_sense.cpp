#include <cstring>
#include "current_sense.h"
#include "adc.h"

void CurrentSense::InitAdc()
{
    HAL_ADCEx_InjectedStart_IT(&hadc1);
}


float CurrentSense::GetAdcToVoltage(Channel_t _channel)
{
    return (float) rawAdcVal[_channel] / 4096 * 3.3f;
}
