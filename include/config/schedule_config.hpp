#pragma once

#include "blocks/scheduler/scheduler.hpp"

// Структура для хранения ID расписаний
struct ScheduleIds {
    // Tank20 (id 1)
    uint8_t tank20_mon_morning;
    uint8_t tank20_mon_evening;
    uint8_t tank20_tue_morning;
    uint8_t tank20_tue_evening;
    uint8_t tank20_wed_morning;
    uint8_t tank20_wed_evening;
    uint8_t tank20_thu_morning;
    uint8_t tank20_thu_evening;
    uint8_t tank20_fri_morning;
    uint8_t tank20_fri_evening;
    uint8_t tank20_sun_morning;
    uint8_t tank20_sun_evening;
    
    // Tank10 (id 2)
    uint8_t tank10_mon;
    uint8_t tank10_tue;
    uint8_t tank10_wed;
    uint8_t tank10_thu;
    uint8_t tank10_fri;
    uint8_t tank10_sun;
};

extern ScheduleIds scheduleIds;

// Объявление функции для настройки расписания
void setupSchedule(Scheduler& scheduler); 