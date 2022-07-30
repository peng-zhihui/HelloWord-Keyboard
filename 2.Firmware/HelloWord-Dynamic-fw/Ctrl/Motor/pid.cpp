#include "pid.h"


float PidController::operator()(float error)
{
    auto time = micros();
    float dt = (float) (time - timeStamp) * 1e-6f;
    // Quick fix for strange cases (micros overflow)
    if (dt <= 0 || dt > 0.5f) dt = 1e-3f;

    float pTerm = p * error;
    float iTerm = integralLast + i * dt * 0.5f * (error + errorLast);
    iTerm = CONSTRAINT(iTerm, -limit, limit);
    float dTerm = d * (error - errorLast) / dt;

    float output = pTerm + iTerm + dTerm;
    output = CONSTRAINT(output, -limit, limit);

    // If output ramp defined
    if (outputRamp > 0)
    {
        // Limit the acceleration by ramping the output
        float outputRate = (output - outputLast) / dt;
        if (outputRate > outputRamp)
            output = outputLast + outputRamp * dt;
        else if (outputRate < -outputRamp)
            output = outputLast - outputRamp * dt;
    }

    integralLast = iTerm;
    outputLast = output;
    errorLast = error;
    timeStamp = time;

    return output;
}
