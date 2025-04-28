#pragma once

#include <Arduino.h>

class Mosfet {
private:
    uint8_t _pin;
    uint8_t _tankId;
    bool _isOn;
    unsigned long _lastToggleTime; // Время последнего переключения
    const unsigned long TOGGLE_LOCKOUT = 200; // Минимальное время между переключениями (мс)

public:
    Mosfet(uint8_t pin, uint8_t tankId);
    void init();
    bool turnOn();  // Возвращает успех операции
    bool turnOff(); // Возвращает успех операции
    bool isOn() const;
    uint8_t getTankId() const;
}; 