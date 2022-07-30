#include "driver_fd6288q.h"


bool DriverFD6288QBase::Init()
{
    return ConfigTimerForPwm();
}


void DriverFD6288QBase::SetVoltage(float _voltageA, float _voltageB, float _voltageC)
{
    _voltageA = CONSTRAINT(_voltageA, 0, voltagePowerSupply);
    _voltageB = CONSTRAINT(_voltageB, 0, voltagePowerSupply);
    _voltageC = CONSTRAINT(_voltageC, 0, voltagePowerSupply);

    dutyA = _voltageA / voltagePowerSupply;
    dutyB = _voltageB / voltagePowerSupply;
    dutyC = _voltageC / voltagePowerSupply;

    SetPwmDutyByRegister(dutyA, dutyB, dutyC);
}


void DriverFD6288QBase::SetEnable(bool _enable)
{
    if (!_enable)
        SetVoltage(0, 0, 0);
}


