#include "BuzzerAlarm.h"
#include <Arduino.h>
#include <math.h>
#define WARNINGINTERVAL 500
#define ERRORINTERVAL 200

BuzzerAlarm::BuzzerAlarm(uint8_t pin){ // bere pin, nastaví ho jako výstup
    this->pin = pin;
    pinMode(pin,OUTPUT);
    digitalWrite(pin, LOW);
}
void BuzzerAlarm::setStatus(BuzzerStatus newStatus){ // co zvenčí řekne "teď hraj tohle".
    if(newStatus != buzzerStatus){
        buzzerStatus = newStatus;
        lastToggleMillis = millis();
        if(newStatus == BuzzerStatus::NORMAL){
            digitalWrite(pin, LOW);
            pinEnabled = false;
        }
    }
    
    return;
}
void BuzzerAlarm::update(){ 
    unsigned long now = millis();
    if(buzzerStatus == BuzzerStatus::NORMAL){
        return;
    }else{
        unsigned long interval = intervalForStatus(buzzerStatus);
        if(now - lastToggleMillis >= interval){
            pinEnabled = !pinEnabled;
            if (pinEnabled) {
                digitalWrite(pin, HIGH);
            } else {
                digitalWrite(pin, LOW);
            }
            lastToggleMillis = now;
        }
    }
}

unsigned long BuzzerAlarm::intervalForStatus(BuzzerStatus status) const{ // podle enum vybírá interval tónu buzzeru
    switch(status){
        case BuzzerStatus::NORMAL: 
            return 0;
        case BuzzerStatus::WARNING:
            return WARNINGINTERVAL;
        case BuzzerStatus::ERROR:
            return ERRORINTERVAL;
    }
}
