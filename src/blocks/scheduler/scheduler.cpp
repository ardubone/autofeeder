#include "blocks/scheduler/scheduler.hpp"

Scheduler::Scheduler() : _clock(nullptr), _mosfetTank20(nullptr), _mosfetTank10(nullptr), _scheduleCount(0) {}

const char* getDayName(uint8_t day) {
    switch(day) {
        case 0: return "Воскресенье";
        case 1: return "Понедельник";
        case 2: return "Вторник";
        case 3: return "Среда";
        case 4: return "Четверг";
        case 5: return "Пятница";
        case 6: return "Суббота";
        default: return "Неизвестный день";
    }
}

void Scheduler::init(Clock* clock, MosfetTank20* mosfetTank20, MosfetTank10* mosfetTank10) {
    _clock = clock;
    _mosfetTank20 = mosfetTank20;
    _mosfetTank10 = mosfetTank10;
    
    Serial.println(F("[SCHEDULER] Инициализация планировщика"));
    Serial.println(F("[SCHEDULER] Текущие расписания:"));
    
    for (uint8_t i = 0; i < _scheduleCount; i++) {
        Serial.print(F("[SCHEDULER] Бак "));
        Serial.print(_schedules[i].tankId);
        Serial.print(F(": "));
        Serial.print(_schedules[i].hour);
        Serial.print(F(":"));
        Serial.print(_schedules[i].minute);
        Serial.print(F(" День недели: "));
        Serial.println(getDayName(_schedules[i].dayOfWeek));
    }
}

void Scheduler::addSchedule(uint8_t hour, uint8_t minute, uint8_t dayOfWeek, uint8_t tankId) {
    if (_scheduleCount < MAX_SCHEDULES) {
        _schedules[_scheduleCount] = {hour, minute, dayOfWeek, tankId};
        _scheduleCount++;
    }
}

bool Scheduler::shouldActivate(uint8_t tankId) {
    if (!_clock || !_mosfetTank20 || !_mosfetTank10) return false;

    DateTime now = _clock->getTime();
    uint8_t currentDay = now.dayOfTheWeek();
    uint8_t currentHour = now.hour();
    uint8_t currentMinute = now.minute();

    // Вывод расписаний для текущего дня
    Serial.println(F("[SCHEDULER] Расписания на сегодня:"));
    for (uint8_t i = 0; i < _scheduleCount; i++) {
        if (_schedules[i].dayOfWeek == currentDay) {
            Serial.print(F("[SCHEDULER] Бак "));
            Serial.print(_schedules[i].tankId);
            Serial.print(F(": "));
            Serial.print(_schedules[i].hour);
            Serial.print(F(":"));
            Serial.println(_schedules[i].minute);
        }
    }

    for (uint8_t i = 0; i < _scheduleCount; i++) {
        if (_schedules[i].tankId == tankId &&
            _schedules[i].dayOfWeek == currentDay &&
            _schedules[i].hour == currentHour &&
            _schedules[i].minute == currentMinute) {
            
            if (tankId == 1 && !_mosfetTank20->isOn()) {
                return true;
            } else if (tankId == 2 && !_mosfetTank10->isOn()) {
                return true;
            }
        }
    }
    
    return false;
} 