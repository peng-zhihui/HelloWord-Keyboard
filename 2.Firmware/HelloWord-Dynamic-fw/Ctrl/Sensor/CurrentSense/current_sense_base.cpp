#include "current_sense_base.h"
#include <cmath>


float CurrentSenseBase::GetDcCurrent(float _angleElectrical)
{
    PhaseCurrent_t current = GetPhaseCurrents();

    // Signal filtering using identity a + b + c = 0.
    // Assumes measurement error is normally distributed.
    float a, b;
    if (pwmDutyA > dropOnePhaseDutyThreshold ||
        pwmDutyB > dropOnePhaseDutyThreshold ||
        pwmDutyC > dropOnePhaseDutyThreshold)
    {
        switch (sector)
        {
            case 1:
            case 6:
                a = -current.b - current.c;
                b = current.b;
                break;
            case 2:
            case 3:
                a = current.a;
                b = -current.a - current.c;
                break;
            case 4:
            case 5:
                a = current.a;
                b = current.b;
                break;
        }
    } else
    {
        float mid = (1.f / 3) * (current.a + current.b + current.c);
        a = current.a - mid;
        b = current.b - mid;
    }

    iAlpha = a;
    iBeta = _1_SQRT3 * a + _2_SQRT3 * b;

    // Determine the sign of the current:
    // sign(atan2(current.q, current.d)) is the same as c.q > 0 ? 1 : -1
    float sign = (iBeta * CosApprox(_angleElectrical) - iAlpha * SinApprox(_angleElectrical)) > 0 ? 1 : -1;

    // return current magnitude
    return sign * SQRT(iAlpha * iAlpha + iBeta * iBeta);
}


DqCurrent_t CurrentSenseBase::GetFocCurrents(float _angleElectrical)
{
    PhaseCurrent_t current = GetPhaseCurrents();

    // Signal filtering using identity a + b + c = 0.
    // Assumes measurement error is normally distributed.
    float a, b;
    if (pwmDutyA > dropOnePhaseDutyThreshold ||
        pwmDutyB > dropOnePhaseDutyThreshold ||
        pwmDutyC > dropOnePhaseDutyThreshold)
    {
        switch (sector)
        {
            case 1:
            case 6:
                a = -current.b - current.c;
                b = current.b;
                break;
            case 2:
            case 3:
                a = current.a;
                b = -current.a - current.c;
                break;
            case 4:
            case 5:
                a = current.a;
                b = current.b;
                break;
        }
    } else
    {
        float mid = (1.f / 3) * (current.a + current.b + current.c);
        a = current.a - mid;
        b = current.b - mid;
    }

    iAlpha = a;
    iBeta = _1_SQRT3 * a + _2_SQRT3 * b;

    // Calculate park transform
    float ct = CosApprox(_angleElectrical);
    float st = SinApprox(_angleElectrical);

    DqCurrent_t focCurrent{};
    focCurrent.d = iAlpha * ct + iBeta * st;
    focCurrent.q = iBeta * ct - iAlpha * st;

    return focCurrent;
}