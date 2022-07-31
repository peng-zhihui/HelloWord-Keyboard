#ifndef CTRL_STEP_FW_LOW_SIDE_CURRENT_SENSE_M0_H
#define CTRL_STEP_FW_LOW_SIDE_CURRENT_SENSE_M0_H

#include "Ctrl/Sensor/CurrentSense/Instances/low_side_current_sense.h"

class CurrentSense : public LowSideCurrentSenseBase
{
public:
    explicit CurrentSense(float _shuntResistor, float _gain)
        : LowSideCurrentSenseBase(_shuntResistor, _gain)
    {}


    uint16_t rawAdcVal[3]{};


private:
    void InitAdc() override;
    float GetAdcToVoltage(Channel_t _channel) override;
};

#endif
