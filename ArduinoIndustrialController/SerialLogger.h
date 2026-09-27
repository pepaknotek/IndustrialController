#pragma once
#include "ILogger.h"

// Nejjednodussi implementace pro prvni fazi portovani: zaznamy jen
// vypisuje na Serial, casovy udaj je zatim jen millis() od startu.
// Realne casove razitko (DS3231 RTC) a ukladani do EEPROM je dalsi krok.
class SerialLogger : public ILogger
{
public:
	void log(const char* sensorName, double value,
	          const char* oldState, const char* newState) override;
};
