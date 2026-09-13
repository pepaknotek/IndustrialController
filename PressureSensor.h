#pragma once
#include "ISensor.h"

class PressureSensor : public ISensor
{
public:
    double readSensor() override;
    void setPressure(double pressure);
private:
    double pressure = 92.4;
};

