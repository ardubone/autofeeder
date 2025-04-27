#pragma once

#include <Arduino.h>
#include "components/clock/clock.hpp"
#include "components/mosfet/mosfet.hpp"

struct Schedule {
    uint8_t hour;
    uint8_t minute;
    uint8_t dayOfWeek;
    uint8_t tankId; // 1 - основной бак (tank20), 2 - tank10
    uint8_t id;     // Уникальный идентификатор расписания
    bool enabled;   // Включено или выключено расписание
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
    uint8_t _nextScheduleId;

    // Поиск расписания по ID
    int findScheduleIndexById(uint8_t id);

public:
    Scheduler();
    void init(Clock* clock);
    void addMosfet(Mosfet* mosfet);
    
    // Методы управления расписанием
    uint8_t addSchedule(uint8_t hour, uint8_t minute, uint8_t dayOfWeek, uint8_t tankId);
    bool removeSchedule(uint8_t id);
    bool enableSchedule(uint8_t id, bool enable);
    bool updateSchedule(uint8_t id, uint8_t hour, uint8_t minute, uint8_t dayOfWeek, uint8_t tankId);
    void clearAllSchedules();
    
    // Методы для получения информации о расписании
    uint8_t getScheduleCount() const;
    Schedule getSchedule(uint8_t index) const;
    void printAllSchedules();
    
    bool shouldActivate(uint8_t tankId);
    Mosfet* getMosfet(uint8_t tankId);
}; 