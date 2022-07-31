#ifndef PID_H
#define PID_H


#include "Platform/Utils/time_utils.h"
#include "math_utils.h"


class PidController
{
public:
    PidController() = default;

    explicit PidController(float _p, float _i, float _d, float _ramp, float _limit) :
        p(_p), i(_i), d(_d), outputRamp(_ramp), limit(_limit)
    {
        timeStamp = micros();
    }


    float operator()(float error);

    float p = 0;
    float i = 0;
    float d = 0;
    float outputRamp = 0;
    float limit = 0;


protected:
    float errorLast = 0;
    float outputLast = 0;
    float integralLast = 0;
    unsigned long timeStamp = 0;
};

#endif