#include "Controller.h"
#ifndef ARDUINO
#include <sstream>
#endif
#define ERRORLOWTEMPERATURE 88
#define WARNINGHIGHTEMPERATURE 90
#define WARNINGLOWTEMPERATURE 69
#define NORMALHIGHTEMPERATURE 71
#define PRESSUREHYSTERESIS 3
#define NORMALPRESSURELOW 90
#define NORMALPRESSUREHIGH 120
#define ERRORLOWPRESSURE 81
#define ERRORHIGHPRESSURE 130

Controller::Controller(ILogger* logger) : logger(logger) {}

SystemState Controller::evaluateTemperature(double temperature, SystemState currentTemperatureState) {
	switch (currentTemperatureState) {
	case SystemState::ERROR:
		if (temperature >= ERRORLOWTEMPERATURE) {
			return SystemState::ERROR;
		}
		else if (temperature < ERRORLOWTEMPERATURE) {
			return SystemState::WARNING;
		}
		else {
			return currentTemperatureState;
		}
	case SystemState::WARNING:
		if (temperature >= WARNINGHIGHTEMPERATURE) {
			return SystemState::ERROR;
		}
		else if(temperature <= ERRORLOWTEMPERATURE && temperature >= WARNINGLOWTEMPERATURE){
			return SystemState::WARNING;
		}
		else if(temperature < WARNINGLOWTEMPERATURE){
			return SystemState::NORMAL;
		}
		else {
			return currentTemperatureState;
		}
	case SystemState::NORMAL:
		if (temperature >= WARNINGHIGHTEMPERATURE) {
			return SystemState::ERROR;
		}
		else if (temperature >= NORMALHIGHTEMPERATURE && temperature < WARNINGHIGHTEMPERATURE) {
			return SystemState::WARNING;
		}
		else if (temperature < NORMALHIGHTEMPERATURE) {
			return SystemState::NORMAL;
		}
		else {
			return currentTemperatureState;
		}
	default:
		return SystemState::ERROR;
	}
}

const char* Controller::toString(SystemState state) {
	switch (state) {
	case SystemState::ERROR:
		return "ERROR";
	case SystemState::WARNING:
		return "WARNING";
	case SystemState::NORMAL:
		return "NORMAL";
	default:
		return "UNKNOWN";
	}
}

void Controller::updateTemperature(double temperature) {
	SystemState stateTemp = evaluateTemperature(temperature, state);
	if (stateTemp != state) {
		
		log(state, temperature, stateTemp);
		state = stateTemp;

	}
}

SystemState Controller::getState() const{
	return state;
}

void Controller::log(SystemState oldstate, double temperature, SystemState stateTemp) {
	if (logger != nullptr) {
		// externi logger (napr. Arduino SerialLogger) - zadne STL retezce/soubory navic,
		// bezpecne i na cilech bez filesystemu a s malo RAM
		logger->log("temperature sensor", temperature, toString(oldstate), toString(stateTemp));
		return;
	}

#ifndef ARDUINO
	// puvodni chovani pro PC build: soubor + konzole (beze zmeny)
	std::ofstream logFile{ "controller.log" };
	std::ostringstream tempStream;
	tempStream << temperature;
	std::string tempStr = tempStream.str();

	std::string str = "temperature sensor, temperature: " + tempStr + " ,old state: " + toString(oldstate) + " ,new state: " + toString(stateTemp) + " ,date and time: " + getTimestamp();
	logHistory.push_back(str);
	while (logHistory.size() > 10) {
		logHistory.pop_front();
	}
	for (const std::string& entry : logHistory) {
		logFile << entry << "\n";
	}
	std::cout << str << std::endl;
#endif
}

PressureState Controller::evaluatePressure(double pressure, PressureState currentPressureState) {
	switch (currentPressureState) {
	case PressureState::NORMAL:
		if (pressure < (NORMALPRESSURELOW)) {
			return PressureState::LOW_WARNING;
		}
		else if (pressure >= (NORMALPRESSUREHIGH)) {
			return PressureState::HIGH_WARNING;
		}
		else {
			return PressureState::NORMAL;
		}
	case PressureState::LOW_WARNING:
		if (pressure < (ERRORLOWPRESSURE)) {
			return PressureState::LOW_ERROR;
		}
		else if (pressure >= (NORMALPRESSURELOW + PRESSUREHYSTERESIS)) {
			return PressureState::NORMAL;
		}
		else {
			return PressureState::LOW_WARNING;
		}
	case PressureState::LOW_ERROR:
		if (pressure >= (ERRORLOWPRESSURE + PRESSUREHYSTERESIS)) {
			return PressureState::LOW_WARNING;
		}
		else {
			return PressureState::LOW_ERROR;
		}
	case PressureState::HIGH_WARNING:
		if (pressure < (NORMALPRESSUREHIGH - PRESSUREHYSTERESIS)) {
			return PressureState::NORMAL;
		}
		else if (pressure >= (ERRORHIGHPRESSURE)) {
			return PressureState::HIGH_ERROR;
		}
		else {
			return PressureState::HIGH_WARNING;
		}
	case PressureState::HIGH_ERROR:
		if (pressure < (ERRORHIGHPRESSURE - PRESSUREHYSTERESIS)) {
			return PressureState::HIGH_WARNING;
		}
		else {
			return PressureState::HIGH_ERROR;
		}
	default:
		return currentPressureState;
	}
}

void Controller::forceFaultState(){
	if(state != SystemState::ERROR){
		log(state, -999.0, SystemState::ERROR);
		state = SystemState::ERROR;
		return;
	}else{
		return;
	}
}

void Controller::updatePressure(double pressure) {
	PressureState stateTemp = evaluatePressure(pressure, pressureState);
	if (stateTemp != pressureState) {
		// POZNAMKA: puvodni kod tady volal log(state, pressure, stateTemp), coz nesedi
		// na typ SystemState a pravdepodobne se to na PC uz nekompilovalo (viz komentar
		// "je treba upravit pro potreby updatePressure" v puvodnim souboru). V ramci
		// teto faze pressure vetev nechavam funkcne nedoreseny - resi se az v dalsim kroku
		// spolu s tlakovym senzorem.
		pressureState = stateTemp;
	}
}
