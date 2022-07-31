#ifndef CTRL_FOC_LITE_FW_MOTOR_H
#define CTRL_FOC_LITE_FW_MOTOR_H

#include "Ctrl/Driver/driver_base.h"
#include "Ctrl/Sensor/Encoder/encoder_base.h"
#include "Ctrl/Sensor/CurrentSense/current_sense_base.h"
#include "lowpass_filter.h"
#include "math_utils.h"
#include "pid.h"

class Motor
{
public:
    explicit Motor(int _polePairs, float _phaseResistance = NOT_SET) :
        polePairs(_polePairs), phaseResistance(_phaseResistance)
    {
        config.controlMode = ANGLE;
        config.voltageUsedForSensorAlign =1.0f;
        config.voltageLimit = 12.0f;
        config.currentLimit = 0.2f;
        config.velocityLimit = 20.0f;

        config.lpfCurrentQ = LowPassFilter{0.005f};
        config.lpfCurrentD = LowPassFilter{0.005f};
        config.lpfVelocity = LowPassFilter{0.1f};
        config.lpfAngle = LowPassFilter{0.03f};
        config.pidCurrentQ = PidController{3, 300.0f, 0.0f, 0, 12.0f};
        config.pidCurrentD = PidController{3, 300.0f, 0.0f, 0, 12.0f};
        config.pidVelocity = PidController{0.5f, 10.0f, 0.0f, 1000.0f, 12.0f};
        config.pidAngle = PidController{20.0f, 0, 0, 0, 20.0f};
    }


    enum ControlMode_t
    {
        TORQUE,
        VELOCITY,
        ANGLE,
        VELOCITY_OPEN_LOOP,
        ANGLE_OPEN_LOOP
    };

    enum Error_t
    {
        NO_ERROR = 0,
        FAILED_TO_NOTICE_MOVEMENT,
        POLE_PAIR_MISMATCH
    };

    struct Config_t
    {
        float voltageLimit{};
        float currentLimit{};
        float velocityLimit{};
        float voltageUsedForSensorAlign{};
        ControlMode_t controlMode = ANGLE;
        LowPassFilter lpfCurrentQ{};
        LowPassFilter lpfCurrentD{};
        LowPassFilter lpfVelocity{};
        LowPassFilter lpfAngle{};
        PidController pidCurrentQ;
        PidController pidCurrentD;
        PidController pidVelocity;
        PidController pidAngle;
    };

    struct State_t
    {
        float rawAngle{};
        float estAngle{};
        float rawVelocity{};
        float estVelocity{};
    };


    bool Init(float _zeroElectricOffset = NOT_SET, EncoderBase::Direction _encoderDir = EncoderBase::CW);
    void AttachDriver(DriverBase* _driver);
    void AttachEncoder(EncoderBase* _encoder);
    void AttachCurrentSense(CurrentSenseBase* _currentSense);
    void SetEnable(bool _enable);
    float GetEstimateAngle();
    float GetEstimateVelocity();
    float GetElectricalAngle();
    void Tick();
    void SetTorqueLimit(float _val);


    float target = 0;
    Error_t error = NO_ERROR;
    Config_t config{};
    State_t state{};
    DqVoltage_t voltage{};
    DqCurrent_t current{};
    float zeroElectricAngleOffset = NOT_SET;
    CurrentSenseBase* currentSense = nullptr;
    DriverBase* driver = nullptr;
    EncoderBase* encoder = nullptr;


private:
    bool InitFOC(float _zeroElectricOffset, EncoderBase::Direction _sensorDirection);
    bool AlignSensor();
    void CloseLoopControlTick();
    void FocOutputTick();
    float VelocityOpenLoopTick(float _target);
    float AngleOpenLoopTick(float _target);
    void SetPhaseVoltage(float _voltageQ, float _voltageD, float _angleElectrical);


    bool enabled = false;
    float phaseResistance = NOT_SET;
    int polePairs = 7;
    float voltageA{}, voltageB{}, voltageC{};
    float estimateAngle{};
    float electricalAngle{};
    float estimateVelocity{};
    float setPointCurrent{};
    float setPointVelocity{};
    float setPointAngle{};
    uint64_t openLoopTimestamp{};
};


#endif
