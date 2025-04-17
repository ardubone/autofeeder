#pragma once

#include <Arduino.h>

class Mosfet {
private:
    uint8_t _pin;
    bool _isOn;

public:
    Mosfet(uint8_t pin) : _pin(pin), _isOn(false) {}

    void init() {
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, LOW);
        Serial.print("[MOSFET] Инициализация MOSFET на пине ");
        Serial.println(_pin);
    }

    void turnOn() {
        digitalWrite(_pin, HIGH);
        _isOn = true;
        Serial.print("[MOSFET] MOSFET включен, состояние пина: ");
        Serial.println(digitalRead(_pin));
    }

    void turnOff() {
        digitalWrite(_pin, LOW);
        _isOn = false;
        Serial.print("[MOSFET] MOSFET выключен, состояние пина: ");
        Serial.println(digitalRead(_pin));
    }

    bool isOn() const {
        bool pinState = digitalRead(_pin);
        Serial.print("[MOSFET] Проверка состояния MOSFET: ");
        Serial.print(_isOn);
        Serial.print(", состояние пина: ");
        Serial.println(pinState);
        return _isOn;
    }
}; 