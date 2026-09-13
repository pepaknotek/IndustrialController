#pragma once
#include "ISensor.h"



class TemperatureSensor : public ISensor
{
public:
    double readSensor() override;
    void setTemperature(double temperature);
private:
    double temperature = 23.7;
};