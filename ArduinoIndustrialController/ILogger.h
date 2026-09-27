#pragma once

// Rozhrani pro logovani prechodu stavu. Ma zamerne "rozlozene" parametry
// (ne uz hotovy std::string), aby implementace na embedded cilech
// (Arduino Uno, 2 kB RAM) nemusela pouzivat std::ostringstream/std::string
// pro sestaveni zpravy - to je hlavni duvod, proc tohle rozhrani vzniklo
// pri portovani puvodniho PC Controlleru na hardware.
class ILogger
{
public:
	virtual ~ILogger() = default;
	virtual void log(const char* sensorName, double value,
	                  const char* oldState, const char* newState) = 0;
};
