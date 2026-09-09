#include "TemperatureSensor.h"

double TemperatureSensor::readTemperature() {
	return temperature;
}

void TemperatureSensor::setTemperature(double temperature) {
	this->temperature = temperature;
}
