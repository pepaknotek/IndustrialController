#include "ArduinoTemperatureSensor.h"
#include <Arduino.h>
#include <math.h>

namespace {
	constexpr int ADC_MAX = 1023;
	constexpr int FAULT_MARGIN = 3; // par LSB od kraje rozsahu = bereme jako poruchu
}

ArduinoTemperatureSensor::ArduinoTemperatureSensor(uint8_t pin,
                                                     double seriesResistorOhm,
                                                     double nominalResistanceOhm,
                                                     double bCoefficient,
                                                     double nominalTemperatureC)
	: pin(pin),
	  seriesResistorOhm(seriesResistorOhm),
	  nominalResistanceOhm(nominalResistanceOhm),
	  bCoefficient(bCoefficient),
	  nominalTemperatureK(nominalTemperatureC + 273.15)
{
	pinMode(pin, INPUT);
}

double ArduinoTemperatureSensor::readSensor()
{
	int adcValue = analogRead(pin);

	if (adcValue <= FAULT_MARGIN || adcValue >= (ADC_MAX - FAULT_MARGIN)) {
		// termistor odpojeny (adc blizko 1023) nebo zkratovany (adc blizko 0)
		faultFlag = true;
	} else {
		faultFlag = false;
	}

	return adcToCelsius(adcValue);
}

bool ArduinoTemperatureSensor::lastReadingWasFault() const
{
	return faultFlag;
}

double ArduinoTemperatureSensor::adcToCelsius(int adcValue) const
{
	if (adcValue <= 0) adcValue = 1;
	if (adcValue >= ADC_MAX) adcValue = ADC_MAX - 1;

	// napetovy delic: pevny rezistor nahore, NTC dole (uprav pri obracenem zapojeni)
	double resistance = seriesResistorOhm / (static_cast<double>(ADC_MAX) / adcValue - 1.0);

	// Steinhart-Hart, zjednodusena B-parametr rovnice
	double steinhart = resistance / nominalResistanceOhm;
	steinhart = log(steinhart);
	steinhart /= bCoefficient;
	steinhart += 1.0 / nominalTemperatureK;
	steinhart = 1.0 / steinhart;
	steinhart -= 273.15;

	return steinhart;
}
