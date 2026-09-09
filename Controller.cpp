#include "Controller.h"
#include <sstream>
#define ERRORLOW 88
#define WARNINGHIGH 90
#define WARNINGLOW 69
#define NORMALHIGH 71


SystemState Controller::evaluate(double temperature, SystemState currentState) {
	switch (currentState) {
	case ERROR:
		if (temperature >= ERRORLOW) {
			return ERROR;
		}
		else if (temperature < ERRORLOW) {
			return WARNING;
		}
		else {
			return currentState;
		}
	case WARNING:
		if (temperature >= WARNINGHIGH) {
			return ERROR;
		}
		else if(temperature <= ERRORLOW && temperature >= WARNINGLOW){
			return WARNING;
		}
		else if(temperature < WARNINGLOW){
			return NORMAL;
		}
		else {
			return currentState;
		}
	case NORMAL:
		if (temperature >= WARNINGHIGH) {
			return ERROR;
		}
		else if (temperature >= NORMALHIGH && temperature < WARNINGHIGH) {
			return WARNING;
		}
		else if (temperature < NORMALHIGH) {
			return NORMAL;
		}
		else {
			return currentState;
		}
	default:
		return ERROR;
	}
}

const char* Controller::toString(SystemState state) {
	switch (state) {
	case ERROR:
		return "ERROR";
	case WARNING:
		return "WARNING";
	case NORMAL:
		return "NORMAL";
	default:
		return "UNKNOWN";
	}
}

void Controller::update(double temperature) {
	SystemState stateTemp = evaluate(temperature, state);
	if (stateTemp != state) {
		log(state, temperature, stateTemp);
		state = stateTemp;
		
	}
}

SystemState Controller::getState() const{
	return state;
}

void Controller::log(SystemState oldstate, double temperature, SystemState stateTemp) {
	std::ofstream logFile{ "controller.log" };
	// změna desetinných míst pro výstup do souboru
	std::ostringstream tempStream;
	tempStream << temperature;
	std::string tempStr = tempStream.str();


	std::string str = "temperature sensor, temperature: " + tempStr + " ,old state: " + toString(oldstate) + " ,new state: " + toString(stateTemp) + " ,date and time: " + getTimestamp() ;
	logHistory.push_back(str);
	while (logHistory.size() > 10) {
		logHistory.pop_front();
	}
	for (const std::string& entry : logHistory) {
		logFile << entry << "\n";
	}
	std::cout << "temperature sensor, temperature: " << tempStr << " ,old state: " << toString(oldstate) <<" ,new state: "<<toString(stateTemp) <<" ,date and time: "<< getTimestamp() << std::endl;
}

