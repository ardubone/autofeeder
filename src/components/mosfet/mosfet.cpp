#include <Arduino.h>
#include "components/mosfet/mosfet.hpp"

Mosfet::Mosfet(uint8_t pin, uint8_t tankId) : _pin(pin), _tankId(tankId), _isOn(false) {}

void Mosfet::init() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    Serial.print(F("[MOSFET] Инициализация MOSFET для бака "));
    Serial.println(_tankId);
}

void Mosfet::turnOn() {
    digitalWrite(_pin, HIGH);
    _isOn = true;
    Serial.print(F("[MOSFET] Включение для бака "));
    Serial.println(_tankId);
}

void Mosfet::turnOff() {
    digitalWrite(_pin, LOW);
    _isOn = false;
    Serial.print(F("[MOSFET] Выключение для бака "));
    Serial.println(_tankId);
}

bool Mosfet::isOn() const {
    return digitalRead(_pin) == HIGH;
}

uint8_t Mosfet::getTankId() const {
    return _tankId;
} 