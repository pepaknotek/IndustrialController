#pragma once
#include "ISensor.h"
#include <stdint.h>

// Realny senzor - NTC termistor na analogovem vstupu Arduina.
// Nahrazuje simulovanou TemperatureSensor pri portovani na hardware,
// beze zmeny v Controlleru (Controller zna jen rozhrani ISensor).
//
// Predpoklada napetovy delic: 5V -- seriovy rezistor -- [ADC pin] -- NTC -- GND
class ArduinoTemperatureSensor : public ISensor
{
public:
	// pin                    = cislo analogoveho vstupu (napr. A0)
	// seriesResistorOhm      = hodnota pevneho rezistoru v delici (napr. 10000 pro 10k)
	// nominalResistanceOhm   = odpor termistoru pri 25 C (dle datasheetu, typicky 10000)
	// bCoefficient           = B-konstanta termistoru (typicky 3950 pro bezne NTC 10k)
	// nominalTemperatureC    = referencni teplota pro nominalResistanceOhm (obvykle 25 C)
	ArduinoTemperatureSensor(uint8_t pin,
	                          double seriesResistorOhm = 10000.0,
	                          double nominalResistanceOhm = 10000.0,
	                          double bCoefficient = 3950.0,
	                          double nominalTemperatureC = 25.0);

	double readSensor() override;

	// true, pokud posledni cteni vypadalo jako odpojeny/zkratovany senzor
	bool lastReadingWasFault() const;

private:
	double adcToCelsius(int adcValue) const;

	uint8_t pin;
	double seriesResistorOhm;
	double nominalResistanceOhm;
	double bCoefficient;
	double nominalTemperatureK;
	bool faultFlag = false;
};
