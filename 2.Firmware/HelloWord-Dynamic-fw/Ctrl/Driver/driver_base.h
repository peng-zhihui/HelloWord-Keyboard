#ifndef DRIVER_BASE_H
#define DRIVER_BASE_H


class DriverBase
{
public:
    explicit DriverBase(float _voltagePowerSupply) :
        voltagePowerSupply(_voltagePowerSupply)
    {}

    float voltagePowerSupply = 12;
    float dutyA = 0;
    float dutyB = 0;
    float dutyC = 0;

    virtual bool Init() = 0;
    virtual void SetEnable(bool _enable) = 0;
    virtual void SetVoltage(float _voltageA, float _voltageB, float _voltageC) = 0;
};

#endif
