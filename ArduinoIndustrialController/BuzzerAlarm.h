#pragma once
#include <stdint.h>

enum class BuzzerStatus
    {
        NORMAL,
        WARNING,
        ERROR
    };

class BuzzerAlarm {
public:
    BuzzerAlarm(uint8_t pin); // bere pin, nastaví ho jako výstup
    void setStatus(BuzzerStatus buzzerStatus); // co zvenčí řekne "teď hraj tohle".
    void update();


private:
    uint8_t pin;
    unsigned long lastToggleMillis = 0;
    bool pinEnabled = false;
    BuzzerStatus buzzerStatus = BuzzerStatus::NORMAL;
    unsigned long intervalForStatus(BuzzerStatus status) const;
};
