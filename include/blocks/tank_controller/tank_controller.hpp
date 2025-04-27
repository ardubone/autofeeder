#pragma once

#include <Arduino.h>
#include "components/button/button.hpp"
#include "components/limit_switch/limit_switch.hpp"
#include "components/mosfet/mosfet.hpp"

class TankController {
private:
    uint8_t _tankId;
    Button* _button;
    LimitSwitch* _limitSwitch;
    Mosfet* _mosfet;
    
    unsigned long _limitIgnoreStartTime;
    bool _isLimitIgnored;
    unsigned long _limitIgnoreTime;
    unsigned long _mosfetDelay;

public:
    TankController(uint8_t tankId, Button* button, LimitSwitch* limitSwitch, Mosfet* mosfet,
                  unsigned long limitIgnoreTime, unsigned long mosfetDelay);
    
    void init();
    void update(unsigned long currentTime);
    void activateByButton();
    void activateBySchedule();
    
    bool isLimitIgnored() const;
    uint8_t getTankId() const;
    Mosfet* getMosfet() const;
}; 