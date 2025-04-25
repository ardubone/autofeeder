#include "mosfet_tank10.hpp"

MosfetTank10::MosfetTank10(uint8_t pin) : _pin(pin), _isOn(false) {}

void MosfetTank10::init() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
}

void MosfetTank10::turnOn() {
    digitalWrite(_pin, HIGH);
    _isOn = true;
}

void MosfetTank10::turnOff() {
    digitalWrite(_pin, LOW);
    _isOn = false;
}

bool MosfetTank10::isOn() const {
    return _isOn;
} 