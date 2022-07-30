#include "low_side_current_sense.h"


LowSideCurrentSenseBase::LowSideCurrentSenseBase(float _shuntResistor, float _gain)
{
    shuntResistor = _shuntResistor;
    ampGain = _gain;

    voltageToAmpRatio = 1.0f / _shuntResistor / _gain; // volts to amps

    gainA = voltageToAmpRatio;
    gainB = voltageToAmpRatio;
    gainC = voltageToAmpRatio;
}


void LowSideCurrentSenseBase::Init()
{
    InitAdc();
    CalibrateOffsets();
}


void LowSideCurrentSenseBase::CalibrateOffsets()
{
    const int calibrationRounds = 1000;

    zeroOffsetA = 0;
    zeroOffsetB = 0;
    zeroOffsetC = 0;

    for (int i = 0; i < calibrationRounds; i++)
    {
        zeroOffsetA += GetAdcToVoltage(CH_A);
        zeroOffsetB += GetAdcToVoltage(CH_B);
        zeroOffsetC += GetAdcToVoltage(CH_C);
        HAL_Delay(1);
    }

    zeroOffsetA = zeroOffsetA / calibrationRounds;
    zeroOffsetB = zeroOffsetB / calibrationRounds;
    zeroOffsetC = zeroOffsetC / calibrationRounds;
}


PhaseCurrent_t LowSideCurrentSenseBase::GetPhaseCurrents()
{
    PhaseCurrent_t current{};
    current.a = (GetAdcToVoltage(CH_A) - zeroOffsetA) * gainA; // Amps
    current.b = (GetAdcToVoltage(CH_B) - zeroOffsetB) * gainB; // Amps
    current.c = (GetAdcToVoltage(CH_C) - zeroOffsetC) * gainC; // Amps

    return current;
}

