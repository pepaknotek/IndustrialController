#include "SerialLogger.h"
#include <Arduino.h>

void SerialLogger::log(const char* sensorName, double value,
                        const char* oldState, const char* newState)
{
	char valueStr[10];
	dtostrf(value, 4, 1, valueStr); // AVR nema spolehlivou podporu %f v snprintf, dtostrf je standardni reseni

	Serial.print(sensorName);
	Serial.print(F(", value: "));
	Serial.print(valueStr);
	Serial.print(F(", old state: "));
	Serial.print(oldState);
	Serial.print(F(", new state: "));
	Serial.print(newState);
	Serial.print(F(", uptime(ms): "));
	Serial.println(millis());
}
