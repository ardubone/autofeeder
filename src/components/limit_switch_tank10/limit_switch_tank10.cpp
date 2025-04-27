#include <Arduino.h>
#include "components/limit_switch_tank10/limit_switch_tank10.hpp"

LimitSwitchTank10::LimitSwitchTank10(uint8_t pin) : _pin(pin), _lastState(false) {}

void LimitSwitchTank10::init() {
    pinMode(_pin, INPUT_PULLUP);
    _lastState = digitalRead(_pin) == LOW;
    Serial.println(F("[LIMIT_SWITCH_TANK10] Init"));
}

bool LimitSwitchTank10::isTriggered() {
    bool currentState = digitalRead(_pin) == LOW;
    if (currentState != _lastState) {
        _lastState = currentState;
        return currentState;
    }
    return false;
}

void LimitSwitchTank10::reset() {
    _lastState = digitalRead(_pin) == LOW;
} 