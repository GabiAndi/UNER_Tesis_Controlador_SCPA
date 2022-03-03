#include "pidcontroller.h"

PIDController::PIDController(QObject *parent)
    : QObject{parent}
{

}

PIDController::~PIDController()
{

}

uint8_t PIDController::getFreq(float setPointOD, float od, float temp)
{
    float error = (setPointOD - od) * 5;

    if (error > 60.0)
        error = 60.0;

    uint8_t freq = error + 30;

    if (freq > 60)
        freq = 60;

    return freq;
}
