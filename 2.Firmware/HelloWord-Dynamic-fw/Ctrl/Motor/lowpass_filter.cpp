#include "lowpass_filter.h"


float LowPassFilter::operator()(float _input)
{
    unsigned long time = micros();
    float dt = ((float) time - (float) timeStamp) * 1e-6f;

    if (dt < 0.0f) dt = 1e-3f;
    else if (dt > 0.3f)
    {
        outputLast = _input;
        timeStamp = time;
        return _input;
    }

    float alpha = timeConstant / (timeConstant + dt);
    float output = alpha * outputLast + (1.0f - alpha) * _input;
    outputLast = output;
    timeStamp = time;

    return output;
}
