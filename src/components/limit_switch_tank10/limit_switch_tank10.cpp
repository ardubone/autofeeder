#include "limit_switch_tank10.hpp"

LimitSwitchTank10::LimitSwitchTank10(uint8_t pin) : _pin(pin), _lastState(false) {}

void LimitSwitchTank10::init() {
    pinMode(_pin, INPUT_PULLUP);
    _lastState = digitalRead(_pin) == LOW;
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