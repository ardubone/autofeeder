#include <Arduino.h>
#include "components/button/button.hpp"

Button::Button(uint8_t pin, uint8_t tankId) : _pin(pin), _tankId(tankId), _lastState(HIGH), _lastDebounceTime(0) {}

void Button::init() {
    pinMode(_pin, INPUT_PULLUP);
    Serial.print(F("[BUTTON] Инициализация кнопки для бака "));
    Serial.println(_tankId);
}

bool Button::isPressed() {
    // Ручное управление отключено
    /*
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
    */
    return false;
}

uint8_t Button::getTankId() const {
    return _tankId;
} 