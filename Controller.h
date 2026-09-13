#pragma once
#include "TimeUtils.h"
#include <fstream>
#include <deque>
#include <string>
#include <iostream>

    enum class SystemState {
        NORMAL,
        WARNING,
        ERROR
    };

    enum class PressureState{
        NORMAL,
        LOW_WARNING,
        LOW_ERROR,
        HIGH_WARNING,
        HIGH_ERROR
    };

class Controller
{
public:
    void updateTemperature(double temperature);
    void updatePressure(double pressure);
    const char* toString(SystemState state);
    SystemState getState() const;
    
private:
    SystemState evaluateTemperature(double temperature,SystemState currentTemperatureState);
    PressureState evaluatePressure(double pressure, PressureState currentPressureState);
    SystemState state = SystemState::NORMAL;
    PressureState pressureState = PressureState::NORMAL;
    void log(SystemState oldstate, double temperature, SystemState stateTemp);
    std::deque<std::string> logHistory;
};

