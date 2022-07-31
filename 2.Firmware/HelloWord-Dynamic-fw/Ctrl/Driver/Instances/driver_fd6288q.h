#ifndef DriverFD6288QBase_h
#define DriverFD6288QBase_h

#include "Ctrl/Driver/driver_base.h"
#include "Ctrl/Motor/math_utils.h"


class DriverFD6288QBase : public DriverBase
{
public:
    explicit DriverFD6288QBase(float _voltagePowerSupply) :
        DriverBase(_voltagePowerSupply)
    {}


    static constexpr uint32_t PWM_FREQUENCY = 20000; // set by STM32CubeMX


    bool Init() override;
    void SetVoltage(float _voltageA, float _voltageB, float _voltageC) override;
    void SetEnable(bool _enable) override;


private:
    virtual bool ConfigTimerForPwm() = 0;
    virtual void SetPwmDutyByRegister(float _dutyA, float _dutyB, float _dutyC) = 0;
};


#endif
