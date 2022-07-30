#include "encoder_base.h"
#include "Ctrl/Motor/math_utils.h"


inline float abs(float _v)
{
    return _v >= 0 ? _v : -_v;
}


void EncoderBase::Update()
{
    float angle = GetRawAngle();
    angleTimestamp = micros();

    float deltaAngle = angle - angleLast;
    // If overflow happened track it as full rotation
    if (abs(deltaAngle) > (0.8f * _2PI))
        rotationCount += (deltaAngle > 0) ? -1 : 1;

    angleLast = angle;
}


float EncoderBase::GetVelocity()
{
    float time = (float) (angleTimestamp - velocityTimestamp) * 1e-6f;
    // Quick fix for strange cases (micros overflow)
    if (time <= 0) time = 1e-3f;

    // velocity calculation
    float vel = ((float) (rotationCount - rotationCountLast) * _2PI + (angleLast - velocityLast)) / time;

    // save variables for future pass
    velocityLast = angleLast;
    rotationCountLast = rotationCount;
    velocityTimestamp = angleTimestamp;

    return vel;
}


void EncoderBase::VarInit()
{
    // Initialize all the internal variables of EncoderBase
    // to ensure a "smooth" startup (without a 'jump' from zero)
    GetRawAngle();
    delayMicroSeconds(1);

    velocityLast = GetRawAngle();
    velocityTimestamp = micros();
    HAL_Delay(1);

    GetRawAngle();
    delayMicroSeconds(1);

    angleLast = GetRawAngle();
    angleTimestamp = micros();
}


float EncoderBase::GetLapAngle()
{
    return angleLast;
}


float EncoderBase::GetFullAngle()
{
    return (float) rotationCount * _2PI + angleLast;
}


int32_t EncoderBase::GetRotationCount()
{
    return rotationCount;
}
