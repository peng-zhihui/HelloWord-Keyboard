#ifndef SENSOR_H
#define SENSOR_H

#include <cinttypes>
#include "stm32f4xx_hal.h"


class EncoderBase
{
public:
    enum Direction
    {
        CW = 1,
        CCW = -1,
        UNKNOWN = 0
    };


    virtual void Init() = 0;
    virtual void Update();
    virtual float GetLapAngle();
    virtual float GetFullAngle();
    virtual float GetVelocity();
    virtual int32_t GetRotationCount();

    Direction countDirection = Direction::UNKNOWN;


protected:
    virtual void VarInit();
    virtual float GetRawAngle() = 0;


    float angleLast = 0;
    uint64_t angleTimestamp = 0;
    float velocityLast = 0;
    uint64_t velocityTimestamp = 0;
    int32_t rotationCount = 0;
    int32_t rotationCountLast = 0;
};

#endif
