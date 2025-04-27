#pragma once

#include <Arduino.h>

class ButtonTank10 {
private:
    uint8_t _pin;
    bool _lastState;
    unsigned long _lastDebounceTime;
    const unsigned long _debounceDelay = 50;

public:
    ButtonTank10(uint8_t pin);
    void init();
    bool isPressed();
}; 