
// Zapojeni (uprav pin, pokud pouzijes jiny nez A0):
//   5V -- rezistor 10k -- (ADC pin A0) -- termistor NTC -- GND

#include "ISensor.h"
#include "ArduinoTemperatureSensor.h"
#include "ILogger.h"
#include "SerialLogger.h"
#include "Controller.h"
#include "BuzzerAlarm.h"

const uint8_t THERMISTOR_PIN = A0;
const uint8_t BUZZER_PIN = A1;
const unsigned long TEMPERATURE_INTERVAL_MS = 1000;
const unsigned long BUZZER_INTERVAL_MS = 10;

ArduinoTemperatureSensor temperatureSensor(THERMISTOR_PIN);
SerialLogger serialLogger;
Controller controller(&serialLogger);
BuzzerAlarm buzzerAlarm(BUZZER_PIN);

unsigned long lastReadMsTemperature = 0;
unsigned long lastReadMsBuzzer = 0;

BuzzerStatus toBuzzerStatus(SystemState state) {
    switch (state) {
        case SystemState::NORMAL:  return BuzzerStatus::NORMAL;
        case SystemState::WARNING: return BuzzerStatus::WARNING;
        case SystemState::ERROR:   return BuzzerStatus::ERROR;
    }
    return BuzzerStatus::ERROR; // fallback, kdyby switch nepokryl vsechny hodnoty
}

void setup() {
	Serial.begin(9600);
	Serial.println(F("Industrial Controller (Arduino) starting..."));
}

void loop() {
	unsigned long now = millis();
	// čtení teploty
	if (now - lastReadMsTemperature >= TEMPERATURE_INTERVAL_MS) {
		lastReadMsTemperature = now;

		double temperature = temperatureSensor.readSensor();
		if(temperatureSensor.lastReadingWasFault()){
			controller.forceFaultState();
			Serial.println(F("WARNING: cteni ADC na kraji rozsahu (mozny odpojeny/zkratovany senzor)"));
			Serial.print(F("Temperature: "));
			Serial.print("N/A (fault)");
		}else{
			
			controller.updateTemperature(temperature);
			Serial.print(F("Temperature: "));
			Serial.print(temperature);
		
		}
		Serial.print(F(" C, state: "));
		Serial.println(controller.toString(controller.getState()));

			
	}
	// varovný buzzer
	if (now - lastReadMsBuzzer >= BUZZER_INTERVAL_MS) {
		lastReadMsBuzzer = now;
		buzzerAlarm.setStatus(toBuzzerStatus(controller.getState()));
		buzzerAlarm.update();
	}
}
