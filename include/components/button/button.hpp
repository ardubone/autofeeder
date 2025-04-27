#pragma once

#include <Arduino.h>

class Button {
private:
    uint8_t _pin;
    uint8_t _tankId;
    bool _lastState;
    unsigned long _lastDebounceTime;
    const unsigned long _debounceDelay = 50;

public:
    Button(uint8_t pin, uint8_t tankId);
    void init();
    bool isPressed();
    uint8_t getTankId() const;
}; 