#pragma once
#include "ILogger.h"
#ifndef ARDUINO
#include "TimeUtils.h"
#include <fstream>
#include <deque>
#include <string>
#include <iostream>
#endif

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
	// logger = volitelny externi logger (napr. SerialLogger na Arduinu).
	// Kdyz zustane nullptr, Controller se chova presne jako drive (log do souboru).
	explicit Controller(ILogger* logger = nullptr);

	void updateTemperature(double temperature);
	void updatePressure(double pressure);
	const char* toString(SystemState state);
	SystemState getState() const;
	void forceFaultState();

private:
	SystemState evaluateTemperature(double temperature,SystemState currentTemperatureState);
	PressureState evaluatePressure(double pressure, PressureState currentPressureState);
	SystemState state = SystemState::NORMAL;
	PressureState pressureState = PressureState::NORMAL;
	void log(SystemState oldstate, double temperature, SystemState stateTemp);
#ifndef ARDUINO
	std::deque<std::string> logHistory;
#endif
	ILogger* logger;
};
