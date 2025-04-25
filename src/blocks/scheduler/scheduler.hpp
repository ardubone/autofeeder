#pragma once

#include <Arduino.h>
#include "components/clock/clock.hpp"
#include "components/mosfet/mosfet.hpp"
#include "components/mosfet_tank10/mosfet_tank10.hpp"

struct Schedule {
    uint8_t hour;
    uint8_t minute;
    uint8_t dayOfWeek;
    uint8_t tankId; // 1 - основной бак, 2 - tank10
};

class Scheduler {
private:
    Clock* _clock;
    Mosfet* _mosfet;
    MosfetTank10* _mosfetTank10;
    static const uint8_t MAX_SCHEDULES = 32;
    Schedule _schedules[MAX_SCHEDULES];
    uint8_t _scheduleCount;

public:
    Scheduler();
    void init(Clock* clock, Mosfet* mosfet, MosfetTank10* mosfetTank10);
    void addSchedule(uint8_t hour, uint8_t minute, uint8_t dayOfWeek, uint8_t tankId);
    bool shouldActivate(uint8_t tankId);
}; 