#include <Arduino.h>
#include "components/button_tank20/button_tank20.hpp"

ButtonTank20::ButtonTank20(uint8_t pin) : _pin(pin), _lastState(false), _lastDebounceTime(0) {}

void ButtonTank20::init() {
    pinMode(_pin, INPUT_PULLUP);
    Serial.println(F("[BUTTON_TANK20] Инициализация кнопки"));
}

bool ButtonTank20::isPressed() {
    bool currentState = digitalRead(_pin) == LOW;
    
    if (currentState != _lastState) {
        _lastDebounceTime = millis();
    }

    if ((millis() - _lastDebounceTime) > _debounceDelay) {
        if (currentState) {
            _lastState = currentState;
            return true;
        }
    }
    
    _lastState = currentState;
    return false;
} 