#pragma once
#include "TimeUtils.h"
#include <fstream>
#include <deque>
#include <string>
#include <iostream>

enum SystemState {
    NORMAL,
    WARNING,
    ERROR
};

class Controller
{
public:
    void update(double temperature);
    const char* toString(SystemState state);
    SystemState getState() const;
    
private:
    SystemState evaluate(double temperature,SystemState currentState);
    SystemState state = NORMAL;
    void log(SystemState oldstate, double temperature, SystemState stateTemp);
    std::deque<std::string> logHistory;
};

