#pragma once

#include <Arduino.h>

class LimitSwitch {
private:
    uint8_t _pin;
    uint8_t _tankId;
    bool _lastState;

public:
    LimitSwitch(uint8_t pin, uint8_t tankId);
    void init();
    void reset();
    bool isTriggered();
    uint8_t getTankId() const;
}; 