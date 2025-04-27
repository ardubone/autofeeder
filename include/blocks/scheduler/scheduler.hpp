#pragma once

#include <Arduino.h>
#include "components/clock/clock.hpp"
#include "components/mosfet_tank20/mosfet_tank20.hpp"
#include "components/mosfet_tank10/mosfet_tank10.hpp"

struct Schedule {
    uint8_t hour;
    uint8_t minute;
    uint8_t dayOfWeek;
    uint8_t tankId; // 1 - основной бак (tank20), 2 - tank10
};

class Scheduler {
private:
    Clock* _clock;
    MosfetTank20* _mosfetTank20;
    MosfetTank10* _mosfetTank10;
    static const uint8_t MAX_SCHEDULES = 32;
    Schedule _schedules[MAX_SCHEDULES];
    uint8_t _scheduleCount;

public:
    Scheduler();
    void init(Clock* clock, MosfetTank20* mosfetTank20, MosfetTank10* mosfetTank10);
    void addSchedule(uint8_t hour, uint8_t minute, uint8_t dayOfWeek, uint8_t tankId);
    bool shouldActivate(uint8_t tankId);
}; 