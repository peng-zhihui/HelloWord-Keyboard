#ifndef CTRL_FOC_LITE_FW_LOW_SIDE_CURRENT_SENSE_H
#define CTRL_FOC_LITE_FW_LOW_SIDE_CURRENT_SENSE_H

#include "stm32f4xx_hal.h"
#include "Ctrl/Sensor/CurrentSense/current_sense_base.h"
#include "Ctrl/Motor/math_utils.h"


class LowSideCurrentSenseBase : public CurrentSenseBase
{
public:
    enum Channel_t
    {
        CH_A = 0,
        CH_B = 1,
        CH_C = 2
    };


    LowSideCurrentSenseBase(float _shuntResistor, float gain);


    void Init() override;
    PhaseCurrent_t GetPhaseCurrents() override;


    float gainA;
    float gainB;
    float gainC;


private:
    virtual void InitAdc() = 0;
    virtual float GetAdcToVoltage(Channel_t _channel) = 0;


    float shuntResistor;
    float ampGain;
    float voltageToAmpRatio;


    void CalibrateOffsets();
    float zeroOffsetA = 0;
    float zeroOffsetB = 0;
    float zeroOffsetC = 0;
};

#endif
