#include <Arduino.h>
#include "components/limit_switch_tank20/limit_switch_tank20.hpp"

LimitSwitchTank20::LimitSwitchTank20(uint8_t pin) : _pin(pin), _lastState(false) {}

void LimitSwitchTank20::init() {
    pinMode(_pin, INPUT_PULLUP);
    _lastState = digitalRead(_pin) == LOW;
    Serial.println(F("[LIMIT_SWITCH_TANK20] Init"));
}

void LimitSwitchTank20::reset() {
    _lastState = digitalRead(_pin) == LOW;
}

bool LimitSwitchTank20::isTriggered() {
    bool currentState = digitalRead(_pin) == LOW;
    if (currentState != _lastState) {
        _lastState = currentState;
        return currentState;
    }
    return false;
} 