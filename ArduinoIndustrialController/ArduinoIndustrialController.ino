// Zapojeni termistoru (uprav pin, pokud pouzijes jiny nez A0):
//   5V -- rezistor 10k -- (ADC pin A0) -- termistor NTC -- GND
//
// Zapojeni bzucaku (aktivni, 2 vyvody):
//   A1 (BUZZER_PIN) -- (+) buzzer (-) -- GND
//
// Zapojeni MCP2515 (SPI):
//   VCC -- 5V, GND -- GND, CS -- D10, SO(MISO) -- D12, SI(MOSI) -- D11, SCK -- D13

#include "ISensor.h"
#include "ArduinoTemperatureSensor.h"
#include "ILogger.h"
#include "SerialLogger.h"
#include "Controller.h"
#include "BuzzerAlarm.h"
#include "CanReporter.h"
#include "StatusLeds.h"

const uint8_t THERMISTOR_PIN = A0;
const uint8_t BUZZER_PIN = A1;
const uint8_t CAN_CS_PIN = 10;
const uint8_t GREENLEDPIN = 2;
const uint8_t YELLOWLEDPIN = 3;
const uint8_t REDLEDPIN = 4;
const unsigned long TEMPERATURE_INTERVAL_MS = 1000;
const unsigned long BUZZER_INTERVAL_MS = 10;

ArduinoTemperatureSensor temperatureSensor(THERMISTOR_PIN);
SerialLogger serialLogger;
Controller controller(&serialLogger);
BuzzerAlarm buzzerAlarm(BUZZER_PIN);
CanReporter canReporter(CAN_CS_PIN);
StatusLeds statusLeds(GREENLEDPIN,YELLOWLEDPIN,REDLEDPIN);

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
LedStatus toLedStatus(SystemState state) {
    switch (state) {
        case SystemState::NORMAL:  return LedStatus::NORMAL;
        case SystemState::WARNING: return LedStatus::WARNING;
        case SystemState::ERROR:   return LedStatus::ERROR;
    }
    return LedStatus::ERROR; // fallback, kdyby switch nepokryl vsechny hodnoty
}

void setup() {
	Serial.begin(9600);
	Serial.println(F("Industrial Controller (Arduino) starting..."));

	if (canReporter.begin()) {
		Serial.println(F("CAN (MCP2515) init OK, loopback mode"));
	} else {
		Serial.println(F("CAN (MCP2515) init FAILED - zkontroluj zapojeni/CS pin"));
	}
}

void loop() {
	unsigned long now = millis();
	// čtení teploty
	if (now - lastReadMsTemperature >= TEMPERATURE_INTERVAL_MS) {
		lastReadMsTemperature = now;

		double temperature = temperatureSensor.readSensor();
		bool sensorFault = temperatureSensor.lastReadingWasFault();
		if(sensorFault){
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
		//vybere a rozsvítí správnou ledku
		statusLeds.setStatus(toLedStatus(controller.getState()));

		canReporter.setData(static_cast<uint8_t>(controller.getState()), sensorFault, temperature);	
	}
	// varovný buzzer
	if (now - lastReadMsBuzzer >= BUZZER_INTERVAL_MS) {
		lastReadMsBuzzer = now;
		buzzerAlarm.setStatus(toBuzzerStatus(controller.getState()));
		buzzerAlarm.update();
	}

	canReporter.update();

	// DOCASNE - overeni loopback testu, dokud neni druhy uzel/analyzator
	unsigned long rxId;
	uint8_t rxBuf[8];
	uint8_t rxLen;
	if (canReporter.checkLoopback(rxId, rxBuf, rxLen)) {
		Serial.print(F("CAN loopback OK, data:"));
		for (uint8_t i = 0; i < rxLen; i++) {
			Serial.print(' ');
			Serial.print(rxBuf[i], HEX);
		}
		Serial.println();
	}
}
