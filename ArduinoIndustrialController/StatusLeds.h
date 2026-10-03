#pragma once
#include <stdint.h>

enum class LedStatus{
    NORMAL,
    WARNING,
    ERROR
};

class StatusLeds{
    public:
    StatusLeds(uint8_t greenPin, uint8_t yellowPin, uint8_t redPin);
    void setStatus(LedStatus status);

    private:
    uint8_t greenPin;
    uint8_t yellowPin;
    uint8_t redPin;
};