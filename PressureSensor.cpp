#include "PressureSensor.h"

double PressureSensor::readSensor() {
	return pressure;
}

void PressureSensor::setPressure(double pressure) {
	this->pressure = pressure;
}