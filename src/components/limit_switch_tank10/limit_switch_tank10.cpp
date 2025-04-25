#include "limit_switch_tank10.hpp"

LimitSwitchTank10::LimitSwitchTank10(uint8_t pin) : _pin(pin), _isTriggered(false) {}

void LimitSwitchTank10::init() {
    pinMode(_pin, INPUT_PULLUP);
}

bool LimitSwitchTank10::isTriggered() {
    _isTriggered = digitalRead(_pin) == LOW;
    return _isTriggered;
}

void LimitSwitchTank10::reset() {
    _isTriggered = false;
} 