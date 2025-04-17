#pragma once

#include <Arduino.h>

class LimitSwitch {
private:
    uint8_t _pin;
    bool _wasTriggered;

public:
    LimitSwitch(uint8_t pin) : _pin(pin), _wasTriggered(false) {}

    void init() {
        pinMode(_pin, INPUT_PULLUP);
        Serial.println("[LIMIT_SWITCH] Инициализация концевика");
    }

    void reset() {
        _wasTriggered = false;
    }

    bool isTriggered() {
        bool currentState = digitalRead(_pin) == LOW;
        if (currentState && !_wasTriggered) {
            _wasTriggered = true;
            return true;
        } else if (!currentState) {
            _wasTriggered = false;
        }
        return false;
    }
}; 