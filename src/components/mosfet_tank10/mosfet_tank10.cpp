#include <Arduino.h>
#include "components/mosfet_tank10/mosfet_tank10.hpp"

MosfetTank10::MosfetTank10(uint8_t pin) : _pin(pin), _isOn(false) {}

void MosfetTank10::init() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    Serial.print(F("[MOSFET_TANK10] Init pin "));
    Serial.println(_pin);
}

void MosfetTank10::turnOn() {
    digitalWrite(_pin, HIGH);
    _isOn = true;
    Serial.print(F("[MOSFET_TANK10] On, pin: "));
    Serial.println(digitalRead(_pin));
}

void MosfetTank10::turnOff() {
    digitalWrite(_pin, LOW);
    _isOn = false;
    Serial.print(F("[MOSFET_TANK10] Off, pin: "));
    Serial.println(digitalRead(_pin));
}

bool MosfetTank10::isOn() const {
    return digitalRead(_pin) == HIGH;
} 