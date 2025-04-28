#include <Arduino.h>
#include "components/limit_switch/limit_switch.hpp"

LimitSwitch::LimitSwitch(uint8_t pin, uint8_t tankId) 
    : _pin(pin), 
      _tankId(tankId), 
      _lastState(false),
      _lastDebounceTime(0),
      _debounceDelay(50),        // 50мс задержка для дебаунсинга
      _stableState(false),
      _requiredReadings(3)       // Требуется 3 одинаковых считывания подряд
{}

void LimitSwitch::init() {
    pinMode(_pin, INPUT_PULLUP);
    // Получаем первоначальное состояние и устанавливаем его как стабильное
    _lastState = digitalRead(_pin) == LOW;
    _stableState = _lastState;
    Serial.print(F("[LIMIT_SWITCH] Инициализация концевика для бака "));
    Serial.println(_tankId);
}

void LimitSwitch::reset() {
    _lastState = digitalRead(_pin) == LOW;
    _stableState = _lastState;
    _lastDebounceTime = 0;
}

bool LimitSwitch::isTriggered() {
    bool currentState = digitalRead(_pin) == LOW;
    unsigned long currentTime = millis();
    
    // Если изменилось состояние относительно последнего считывания
    if (currentState != _lastState) {
        _lastState = currentState;
        _lastDebounceTime = currentTime;
        return false; // Возвращаем false до подтверждения стабильного состояния
    }
    
    // Если состояние стабильно в течение времени дебаунсинга
    if ((currentTime - _lastDebounceTime) > _debounceDelay) {
        // Если новое стабильное состояние отличается от предыдущего стабильного
        if (currentState != _stableState) {
            _stableState = currentState;
            // Возвращаем true только если состояние изменилось на активное (LOW)
            return _stableState;
        }
    }
    
    return false;
}

uint8_t LimitSwitch::getTankId() const {
    return _tankId;
} 