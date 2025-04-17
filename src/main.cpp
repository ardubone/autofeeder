#include <Arduino.h>
#include "components/clock/clock.hpp"
#include "components/button/button.hpp"
#include "components/limit_switch/limit_switch.hpp"
#include "components/mosfet/mosfet.hpp"
#include "blocks/scheduler/scheduler.hpp"

// Инициализация компонентов
Clock clock;
Button button(2); // Пин 2 для кнопки
LimitSwitch limitSwitch(3); // Пин 3 для концевика
Mosfet mosfet(4); // Пин 4 для MOSFET
Scheduler scheduler;

// Тестовый режим
const bool TEST_MODE = false;
const unsigned long MOSFET_DELAY = 1000; // Задержка после включения MOSFET в мс
const unsigned long MOSFET_OFF_DELAY = 200; // Задержка после выключения MOSFET в мс
const unsigned long LIMIT_IGNORE_TIME = 5000; // Время игнорирования концевика в мс
const unsigned long SCHEDULE_CHECK_INTERVAL = 45000; // Интервал проверки расписания в мс

// Таймеры
unsigned long _limitIgnoreStartTime = 0;
bool _isLimitIgnored = false;
unsigned long _lastScheduleCheck = 0;

void setup() {
    Serial.begin(9600);
    Serial.println(F("[MAIN] Init"));
    
    clock.init();
    button.init();
    limitSwitch.init();
    mosfet.init();
    scheduler.init(&clock, &mosfet);
    
    // Добавляем расписание
    // Понедельник
    scheduler.addSchedule(9, 0, 1, 1);
    scheduler.addSchedule(19, 0, 1, 1);
    
    // Вторник
    scheduler.addSchedule(9, 0, 2, 1);
    scheduler.addSchedule(19, 0, 2, 1);
    
    // Среда
    scheduler.addSchedule(9, 0, 3, 1);
    scheduler.addSchedule(19, 0, 3, 1);
    
    // Четверг
    scheduler.addSchedule(9, 0, 4, 1);
    scheduler.addSchedule(19, 0, 4, 1);
    
    // Пятница
    scheduler.addSchedule(9, 0, 5, 1);
    scheduler.addSchedule(19, 0, 5, 1);
    
    // Воскресенье
    scheduler.addSchedule(9, 0, 7, 1);
    scheduler.addSchedule(19, 0, 7, 1);
    
    // Установка времени закомментирована, так как время уже установлено вручную
    // DateTime currentTime(2024, 1, 1, 12, 0, 0);
    // clock.setTime(currentTime);
    
    // Выводим текущее время
    DateTime currentTime = clock.getTime();
    Serial.print(F("[MAIN] Current time: "));
    Serial.print(currentTime.year());
    Serial.print(F("-"));
    Serial.print(currentTime.month());
    Serial.print(F("-"));
    Serial.print(currentTime.day());
    Serial.print(F(" "));
    Serial.print(currentTime.hour());
    Serial.print(F(":"));
    Serial.print(currentTime.minute());
    Serial.print(F(":"));
    Serial.println(currentTime.second());
}

void loop() {
    unsigned long currentTime = millis();
    
    // Проверка таймера игнорирования концевика
    if (_isLimitIgnored && (currentTime - _limitIgnoreStartTime) > LIMIT_IGNORE_TIME) {
        _isLimitIgnored = false;
        Serial.println(F("[MAIN] End ignore"));
    }

    if (TEST_MODE) {
        // Тест кнопки и MOSFET
        if (button.isPressed() && !mosfet.isOn()) {
            Serial.println(F("[MAIN] Button press"));
            mosfet.turnOn();
            delay(MOSFET_DELAY);
            limitSwitch.reset();
            _limitIgnoreStartTime = currentTime;
            _isLimitIgnored = true;
            Serial.println(F("[MAIN] Start ignore"));
        }

        // Тест концевика
        if (!_isLimitIgnored && mosfet.isOn() && limitSwitch.isTriggered()) {
            Serial.println(F("[MAIN] Limit switch triggered"));
            Serial.print(F("[MAIN] MOSFET state before: "));
            Serial.println(mosfet.isOn());
            mosfet.turnOff();
            Serial.print(F("[MAIN] MOSFET state after: "));
            Serial.println(mosfet.isOn());
        }

        // Тест расписания с интервалом
        if ((currentTime - _lastScheduleCheck) >= SCHEDULE_CHECK_INTERVAL) {
            _lastScheduleCheck = currentTime;
            if (scheduler.shouldActivate() && !mosfet.isOn()) {
                Serial.println(F("[MAIN] Schedule"));
                mosfet.turnOn();
                delay(MOSFET_DELAY);
                limitSwitch.reset();
                _limitIgnoreStartTime = currentTime;
                _isLimitIgnored = true;
                Serial.println(F("[MAIN] Start ignore"));
            }
        }
    } else {
        // Нормальный режим работы
        if (button.isPressed() && !mosfet.isOn()) {
            Serial.println(F("[MAIN] Button press"));
            mosfet.turnOn();
            delay(MOSFET_DELAY);
            limitSwitch.reset();
            _limitIgnoreStartTime = currentTime;
            _isLimitIgnored = true;
            Serial.println(F("[MAIN] Start ignore"));
        }

        // Проверка концевика
        if (!_isLimitIgnored && mosfet.isOn() && limitSwitch.isTriggered()) {
            Serial.println(F("[MAIN] Limit switch triggered"));
            Serial.print(F("[MAIN] MOSFET state before: "));
            Serial.println(mosfet.isOn());
            mosfet.turnOff();
            Serial.print(F("[MAIN] MOSFET state after: "));
            Serial.println(mosfet.isOn());
        }

        // Проверка расписания с интервалом
        if ((currentTime - _lastScheduleCheck) >= SCHEDULE_CHECK_INTERVAL) {
            _lastScheduleCheck = currentTime;
            if (scheduler.shouldActivate() && !mosfet.isOn()) {
                Serial.println(F("[MAIN] Schedule"));
                mosfet.turnOn();
                delay(MOSFET_DELAY);
                limitSwitch.reset();
                _limitIgnoreStartTime = currentTime;
                _isLimitIgnored = true;
                Serial.println(F("[MAIN] Start ignore"));
            }
        }
    }
    delay(100);
}