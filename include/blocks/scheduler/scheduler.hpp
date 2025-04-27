#pragma once

#include <Arduino.h>
#include "components/clock/clock.hpp"
#include "components/mosfet/mosfet.hpp"

struct Schedule {
    uint8_t hour;
    uint8_t minute;
    uint8_t dayOfWeek;
    uint8_t tankId; // 1 - основной бак (tank20), 2 - tank10
};

class Scheduler {
private:
    Clock* _clock;
    static const uint8_t MAX_SCHEDULES = 32;
    static const uint8_t MAX_TANKS = 8;
    Schedule _schedules[MAX_SCHEDULES];
    uint8_t _scheduleCount;
    Mosfet* _mosfets[MAX_TANKS];
    uint8_t _mosfetCount;

public:
    Scheduler();
    void init(Clock* clock);
    void addMosfet(Mosfet* mosfet);
    void addSchedule(uint8_t hour, uint8_t minute, uint8_t dayOfWeek, uint8_t tankId);
    bool shouldActivate(uint8_t tankId);
    Mosfet* getMosfet(uint8_t tankId);
}; 