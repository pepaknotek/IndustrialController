#include "TemperatureSensor.h"

double TemperatureSensor::readSensor() {
	return temperature;
}

void TemperatureSensor::setTemperature(double temperature) {
	this->temperature = temperature;
}
