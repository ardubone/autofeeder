#include <Arduino.h>
#include "components/limit_switch/limit_switch.hpp"

LimitSwitch::LimitSwitch(uint8_t pin, uint8_t tankId) : _pin(pin), _tankId(tankId), _lastState(false) {}

void LimitSwitch::init() {
    pinMode(_pin, INPUT_PULLUP);
    _lastState = digitalRead(_pin) == LOW;
    Serial.print(F("[LIMIT_SWITCH] Инициализация концевика для бака "));
    Serial.println(_tankId);
}

void LimitSwitch::reset() {
    _lastState = digitalRead(_pin) == LOW;
}

bool LimitSwitch::isTriggered() {
    bool currentState = digitalRead(_pin) == LOW;
    if (currentState != _lastState) {
        _lastState = currentState;
        return currentState;
    }
    return false;
}

uint8_t LimitSwitch::getTankId() const {
    return _tankId;
} 