#pragma once

#include <Arduino.h>

class LimitSwitch {
private:
    uint8_t _pin;
    uint8_t _tankId;
    bool _lastState;
    unsigned long _lastDebounceTime;  // Время последнего изменения состояния
    unsigned long _debounceDelay;     // Задержка дебаунсинга
    bool _stableState;                // Стабильное состояние после дебаунсинга
    uint8_t _requiredReadings;        // Количество считываний для подтверждения

public:
    LimitSwitch(uint8_t pin, uint8_t tankId);
    void init();
    void reset();
    bool isTriggered();
    uint8_t getTankId() const;
}; 