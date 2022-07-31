#ifndef FOCUTILS_LIB_H
#define FOCUTILS_LIB_H

#include "Platform/Utils/time_utils.h"


#define SIGN(a) ( ( (a) < 0 )  ?  -1   : ( (a) > 0 ) )
#define ROUND(x) ((x)>=0?(long)((x)+0.5f):(long)((x)-0.5f))
#define CONSTRAINT(amt, low, high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define SQRT(a) (SqrtApprox(a))
#define ASSERT(a) ( (a) != (NOT_SET) )

#define _2_SQRT3 1.15470053838f
#define _SQRT3 1.73205080757f
#define _1_SQRT3 0.57735026919f
#define _SQRT3_2 0.86602540378f
#define _SQRT2 1.41421356237f
#define _120_D2R 2.09439510239f
#define _PI 3.14159265359f
#define _PI_2 1.57079632679f
#define _PI_3 1.0471975512f
#define _2PI 6.28318530718f
#define _3PI_2 4.71238898038f
#define _PI_6 0.52359877559f

#define NOT_SET -12345.0


struct DqCurrent_t
{
    float d = 0;
    float q = 0;
};


struct PhaseCurrent_t
{
    float a = 0;
    float b = 0;
    float c = 0;
};


struct DqVoltage_t
{
    float d = 0;
    float q = 0;
};


float Normalize(float angle);
float SinApprox(float a);
float CosApprox(float a);
float SqrtApprox(float value);


#endif