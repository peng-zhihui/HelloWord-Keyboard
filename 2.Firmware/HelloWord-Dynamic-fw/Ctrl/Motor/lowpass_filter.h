#ifndef LOWPASS_FILTER_H
#define LOWPASS_FILTER_H


#include "Platform/Utils/time_utils.h"
#include "math_utils.h"


class LowPassFilter
{
public:
    LowPassFilter() = default;

    explicit LowPassFilter(float _timeConstant)
        : timeConstant(_timeConstant), outputLast(0.0f)
    {
        timeStamp = micros();
    }


    float operator()(float _input);
    float timeConstant;


protected:
    unsigned long timeStamp;
    float outputLast;
};

#endif