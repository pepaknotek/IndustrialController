#pragma once

class ITemperatureSensor
{
public:
    virtual ~ITemperatureSensor() = default;
    virtual double readTemperature() = 0;
};

