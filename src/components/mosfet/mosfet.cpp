#include <Arduino.h>
#include "components/mosfet/mosfet.hpp"

Mosfet::Mosfet(uint8_t pin, uint8_t tankId) : _pin(pin), _tankId(tankId), _isOn(false), _lastToggleTime(0) {}

void Mosfet::init() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    _isOn = false;
    _lastToggleTime = millis();
    Serial.print(F("[MOSFET] Инициализация MOSFET для бака "));
    Serial.println(_tankId);
}

bool Mosfet::turnOn() {
    unsigned long currentTime = millis();
    
    // Проверка минимального времени между переключениями
    if (currentTime - _lastToggleTime < TOGGLE_LOCKOUT) {
        Serial.print(F("[MOSFET] Игнорирование слишком частого включения для бака "));
        Serial.println(_tankId);
        return false;
    }
    
    // Дополнительная проверка текущего состояния
    if (digitalRead(_pin) == HIGH) {
        Serial.print(F("[MOSFET] MOSFET уже включен для бака "));
        Serial.println(_tankId);
        _isOn = true;
        return true;
    }
    
    digitalWrite(_pin, HIGH);
    _isOn = true;
    _lastToggleTime = currentTime;
    
    Serial.print(F("[MOSFET] Включение для бака "));
    Serial.println(_tankId);
    return true;
}

bool Mosfet::turnOff() {
    unsigned long currentTime = millis();
    
    // Проверка минимального времени между переключениями
    if (currentTime - _lastToggleTime < TOGGLE_LOCKOUT) {
        Serial.print(F("[MOSFET] Игнорирование слишком частого выключения для бака "));
        Serial.println(_tankId);
        return false;
    }
    
    // Дополнительная проверка текущего состояния
    if (digitalRead(_pin) == LOW) {
        Serial.print(F("[MOSFET] MOSFET уже выключен для бака "));
        Serial.println(_tankId);
        _isOn = false;
        return true;
    }
    
    digitalWrite(_pin, LOW);
    _isOn = false;
    _lastToggleTime = currentTime;
    
    Serial.print(F("[MOSFET] Выключение для бака "));
    Serial.println(_tankId);
    return true;
}

bool Mosfet::isOn() const {
    return digitalRead(_pin) == HIGH;
}

uint8_t Mosfet::getTankId() const {
    return _tankId;
} 