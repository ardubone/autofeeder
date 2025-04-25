#pragma once

#include <Arduino.h>

class LimitSwitchTank10 {
private:
    uint8_t _pin;
    bool _isTriggered;

public:
    LimitSwitchTank10(uint8_t pin);
    void init();
    bool isTriggered();
    void reset();
}; 