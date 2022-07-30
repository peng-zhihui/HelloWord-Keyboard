#ifndef CTRL_FOC_LITE_FW_DRIVER_M0_H
#define CTRL_FOC_LITE_FW_DRIVER_M0_H

#include "Ctrl/Driver/Instances/driver_fd6288q.h"


class Driver : public DriverFD6288QBase
{
public:
    // Configured in STM32CubeMX
    static const int PERIOD_COUNT = 4199;


    explicit Driver(float _voltagePowerSupply) :
        DriverFD6288QBase(_voltagePowerSupply)
    {}


private:
    /***** Platform Specified Implements *****/
    bool ConfigTimerForPwm() override;
    void SetPwmDutyByRegister(float _dutyA, float _dutyB, float _dutyC) override;
};

#endif
