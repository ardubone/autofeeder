#pragma once

#include "components/clock/clock.hpp"
#include "components/mosfet/mosfet.hpp"

struct ScheduleTime {
    uint8_t hour;
    uint8_t minute;
    uint8_t dayOfWeek;
    uint8_t repeatCount; // Количество повторений
    uint8_t currentRepeat; // Текущее повторение
    bool isCompleted; // Флаг выполнения
    unsigned long lastActivationTime; // Время последней активации
    const unsigned long REPEAT_INTERVAL = 60000; // Интервал между повторениями в мс (1 минута)
};

class Scheduler {
private:
    static const uint8_t MAX_SCHEDULES = 10; // Максимальное количество расписаний
    Clock* _clock;
    Mosfet* _mosfet;
    ScheduleTime _schedule[MAX_SCHEDULES];
    uint8_t _scheduleCount;
    bool _wasActivated;

public:
    Scheduler() : _clock(nullptr), _mosfet(nullptr), _scheduleCount(0), _wasActivated(false) {}

    void init(Clock* clk, Mosfet* mosfet) {
        _clock = clk;
        _mosfet = mosfet;
        Serial.println("[SCHEDULER] Инициализация планировщика");
    }

    void addSchedule(uint8_t hour, uint8_t minute, uint8_t dayOfWeek, uint8_t repeatCount = 1) {
        if (_scheduleCount >= MAX_SCHEDULES) {
            Serial.println("[SCHEDULER] Достигнут лимит расписаний");
            return;
        }
        
        Serial.print("[SCHEDULER] Добавление расписания: ");
        Serial.print(hour);
        Serial.print(":");
        Serial.print(minute);
        Serial.print(" День недели: ");
        Serial.print(dayOfWeek);
        Serial.print(" Повторений: ");
        Serial.println(repeatCount);
        
        ScheduleTime& sched = _schedule[_scheduleCount];
        sched.hour = hour;
        sched.minute = minute;
        sched.dayOfWeek = dayOfWeek;
        sched.repeatCount = repeatCount;
        sched.currentRepeat = 0;
        sched.isCompleted = false;
        sched.lastActivationTime = 0;
        
        _scheduleCount++;
    }

    bool shouldActivate() {
        if (!_clock || !_mosfet) return false;
        
        DateTime now = _clock->getTime();
        uint8_t currentDay = _clock->getDayOfWeek();
        unsigned long currentMillis = millis();

        Serial.print("[SCHEDULER] Текущее время: ");
        Serial.print(now.hour());
        Serial.print(":");
        Serial.print(now.minute());
        Serial.print(" День недели: ");
        Serial.println(currentDay);

        for (uint8_t i = 0; i < _scheduleCount; i++) {
            ScheduleTime& sched = _schedule[i];
            
            // Сброс флага выполнения в начале нового дня
            if (now.hour() == 0 && now.minute() == 0) {
                sched.isCompleted = false;
                sched.currentRepeat = 0;
                sched.lastActivationTime = 0;
                Serial.print("[SCHEDULER] Сброс расписания ");
                Serial.println(i + 1);
            }

            // Проверка времени для активации
            bool isTimeMatch = sched.dayOfWeek == currentDay && 
                             sched.hour == now.hour() && 
                             sched.minute == now.minute();

            // Проверка интервала между повторениями
            bool canRepeat = sched.currentRepeat > 0 && 
                           (currentMillis - sched.lastActivationTime) >= sched.REPEAT_INTERVAL;

            Serial.print("[SCHEDULER] Проверка расписания ");
            Serial.print(i + 1);
            Serial.print(": время=");
            Serial.print(isTimeMatch);
            Serial.print(", повтор=");
            Serial.print(canRepeat);
            Serial.print(", выполнено=");
            Serial.print(sched.isCompleted);
            Serial.print(", MOSFET=");
            Serial.println(_mosfet->isOn());

            if (!sched.isCompleted && (isTimeMatch || canRepeat)) {
                if (!_wasActivated && !_mosfet->isOn()) {
                    Serial.print("[SCHEDULER] Активация по расписанию ");
                    Serial.print(i + 1);
                    Serial.print(". Повторение ");
                    Serial.print(sched.currentRepeat + 1);
                    Serial.print(" из ");
                    Serial.println(sched.repeatCount);
                    
                    _wasActivated = true;
                    sched.currentRepeat++;
                    sched.lastActivationTime = currentMillis;
                    
                    if (sched.currentRepeat >= sched.repeatCount) {
                        sched.isCompleted = true;
                        Serial.print("[SCHEDULER] Расписание ");
                        Serial.print(i + 1);
                        Serial.println(" выполнено");
                    }
                    
                    return true;
                }
            } else if (!isTimeMatch && !canRepeat) {
                _wasActivated = false;
            }
        }
        return false;
    }
}; 