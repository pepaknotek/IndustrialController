#pragma once
#include "ITemperatureSensor.h"



class TemperatureSensor : public ITemperatureSensor
{
public:
    double readTemperature() override;
    void setTemperature(double temperature);
private:
    double temperature = 23.7;
};