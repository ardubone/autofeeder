#include <Arduino.h>
#include "components/mosfet_tank20/mosfet_tank20.hpp"

MosfetTank20::MosfetTank20(uint8_t pin) : _pin(pin), _isOn(false) {}

void MosfetTank20::init() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    Serial.print(F("[MOSFET_TANK20] Init pin "));
    Serial.println(_pin);
}

void MosfetTank20::turnOn() {
    digitalWrite(_pin, HIGH);
    _isOn = true;
    Serial.print(F("[MOSFET_TANK20] On, pin: "));
    Serial.println(digitalRead(_pin));
}

void MosfetTank20::turnOff() {
    digitalWrite(_pin, LOW);
    _isOn = false;
    Serial.print(F("[MOSFET_TANK20] Off, pin: "));
    Serial.println(digitalRead(_pin));
}

bool MosfetTank20::isOn() const {
    return digitalRead(_pin) == HIGH;
} 