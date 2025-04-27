#pragma once

#include <Arduino.h>

class MosfetTank20 {
private:
    uint8_t _pin;
    bool _isOn;

public:
    MosfetTank20(uint8_t pin);
    void init();
    void turnOn();
    void turnOff();
    bool isOn() const;
}; 