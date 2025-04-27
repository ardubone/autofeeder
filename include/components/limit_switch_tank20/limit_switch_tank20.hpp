#pragma once

#include <Arduino.h>

class LimitSwitchTank20 {
private:
    uint8_t _pin;
    bool _lastState;

public:
    LimitSwitchTank20(uint8_t pin);
    void init();
    void reset();
    bool isTriggered();
}; 