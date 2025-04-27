#pragma once

#include <Arduino.h>

class Mosfet {
private:
    uint8_t _pin;
    uint8_t _tankId;
    bool _isOn;

public:
    Mosfet(uint8_t pin, uint8_t tankId);
    void init();
    void turnOn();
    void turnOff();
    bool isOn() const;
    uint8_t getTankId() const;
}; 