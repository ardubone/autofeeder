#include "button_tank10.hpp"

ButtonTank10::ButtonTank10(uint8_t pin) : _pin(pin), _lastState(HIGH), _lastDebounceTime(0) {}

void ButtonTank10::init() {
    pinMode(_pin, INPUT_PULLUP);
}

bool ButtonTank10::isPressed() {
    bool currentState = digitalRead(_pin);
    
    if (currentState != _lastState) {
        _lastDebounceTime = millis();
    }
    
    if ((millis() - _lastDebounceTime) > DEBOUNCE_DELAY) {
        if (currentState == LOW) {
            _lastState = currentState;
            return true;
        }
    }
    
    _lastState = currentState;
    return false;
} 