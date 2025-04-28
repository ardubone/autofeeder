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
    bool currentPinState = (digitalRead(_pin) == HIGH);
    if (currentPinState) {
        Serial.print(F("[MOSFET] MOSFET уже включен для бака "));
        Serial.println(_tankId);
        _isOn = true;
        return true;
    }
    
    // Надежное включение с проверкой
    digitalWrite(_pin, HIGH);
    delay(5); // Небольшая задержка для стабилизации
    
    // Проверка, что сигнал действительно установлен
    currentPinState = (digitalRead(_pin) == HIGH);
    if (!currentPinState) {
        // Повторная попытка включения
        Serial.print(F("[MOSFET] Повторная попытка включения для бака "));
        Serial.println(_tankId);
        digitalWrite(_pin, HIGH);
        delay(10); // Увеличенная задержка
        
        // Финальная проверка
        currentPinState = (digitalRead(_pin) == HIGH);
        if (!currentPinState) {
            Serial.print(F("[MOSFET] Не удалось установить HIGH на пине для бака "));
            Serial.println(_tankId);
            return false;
        }
    }
    
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
    bool currentPinState = (digitalRead(_pin) == LOW);
    if (currentPinState) {
        Serial.print(F("[MOSFET] MOSFET уже выключен для бака "));
        Serial.println(_tankId);
        _isOn = false;
        return true;
    }
    
    // Надежное выключение с проверкой
    digitalWrite(_pin, LOW);
    delay(5); // Небольшая задержка для стабилизации
    
    // Проверка, что сигнал действительно сброшен
    currentPinState = (digitalRead(_pin) == LOW);
    if (!currentPinState) {
        // Повторная попытка выключения
        Serial.print(F("[MOSFET] Повторная попытка выключения для бака "));
        Serial.println(_tankId);
        digitalWrite(_pin, LOW);
        delay(10); // Увеличенная задержка
        
        // Финальная проверка
        currentPinState = (digitalRead(_pin) == LOW);
        if (!currentPinState) {
            Serial.print(F("[MOSFET] Не удалось установить LOW на пине для бака "));
            Serial.println(_tankId);
            return false;
        }
    }
    
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