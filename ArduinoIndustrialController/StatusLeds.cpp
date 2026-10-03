#include "StatusLeds.h"
#include <Arduino.h>

StatusLeds::StatusLeds(uint8_t greenPin, uint8_t yellowPin, uint8_t redPin){
    this->greenPin = greenPin;
    this->yellowPin = yellowPin;
    this->redPin = redPin;
    pinMode(greenPin,OUTPUT);
    pinMode(yellowPin,OUTPUT);
    pinMode(redPin,OUTPUT);
    digitalWrite(greenPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(redPin, LOW);
}

void StatusLeds::setStatus(LedStatus status){
    switch(status){
        case LedStatus::NORMAL:
            digitalWrite(greenPin, HIGH);
            digitalWrite(yellowPin, LOW);
            digitalWrite(redPin, LOW);
            break;
        case LedStatus::WARNING:
            digitalWrite(greenPin, LOW);
            digitalWrite(yellowPin, HIGH);
            digitalWrite(redPin, LOW);
            break;
        case LedStatus::ERROR:
            digitalWrite(greenPin, LOW);
            digitalWrite(yellowPin, LOW);
            digitalWrite(redPin, HIGH);
            break;
        default:
            digitalWrite(greenPin, LOW);
            digitalWrite(yellowPin, LOW);
            digitalWrite(redPin, LOW);
            break;
    }
}