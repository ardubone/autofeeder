#include <Arduino.h>
#include "components/clock/clock.hpp"
#include "components/button/button.hpp"
#include "components/limit_switch/limit_switch.hpp"
#include "components/mosfet/mosfet.hpp"
#include "components/button_tank10/button_tank10.hpp"
#include "components/limit_switch_tank10/limit_switch_tank10.hpp"
#include "components/mosfet_tank10/mosfet_tank10.hpp"
#include "blocks/scheduler/scheduler.hpp"

// Инициализация компонентов
Clock clock;
Button button(2); // Пин 2 для кнопки
LimitSwitch limitSwitch(3); // Пин 3 для концевика
Mosfet mosfet(4); // Пин 4 для MOSFET
ButtonTank10 buttonTank10(5); // Пин 5 для кнопки tank10
LimitSwitchTank10 limitSwitchTank10(6); // Пин 6 для концевика tank10
MosfetTank10 mosfetTank10(7); // Пин 7 для MOSFET tank10
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
unsigned long _limitIgnoreStartTimeTank10 = 0;
bool _isLimitIgnoredTank10 = false;
unsigned long _lastScheduleCheck = 0;

void setup() {
    Serial.begin(9600);
    Serial.println(F("[MAIN] Init"));
    
    clock.init();
    button.init();
    limitSwitch.init();
    mosfet.init();
    buttonTank10.init();
    limitSwitchTank10.init();
    mosfetTank10.init();
    scheduler.init(&clock, &mosfet, &mosfetTank10);
    
    // Установка времени
    // DateTime currentTime(2025, 1, 1, 12, 0, 0);
    // clock.setTime(currentTime);
    
    // Добавляем расписание для основного бака
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
    scheduler.addSchedule(9, 0, 0, 1);
    scheduler.addSchedule(19, 0, 0, 1);
    
    // Добавляем расписание для tank10
    // Понедельник
    scheduler.addSchedule(20, 0, 1, 2);
    
    // Вторник
    scheduler.addSchedule(20, 0, 2, 2);
    
    // Среда
    scheduler.addSchedule(20, 0, 3, 2);
    
    // Четверг
    scheduler.addSchedule(20, 0, 4, 2);
    
    // Пятница
    scheduler.addSchedule(20, 0, 5, 2);
    
    // Воскресенье
    scheduler.addSchedule(20, 0, 0, 2);
    
    // Выводим текущее время
    DateTime now = clock.getTime();
    Serial.print(F("[MAIN] Current time: "));
    Serial.print(now.year());
    Serial.print(F("-"));
    Serial.print(now.month());
    Serial.print(F("-"));
    Serial.print(now.day());
    Serial.print(F(" "));
    Serial.print(now.hour());
    Serial.print(F(":"));
    Serial.print(now.minute());
    Serial.print(F(":"));
    Serial.println(now.second());
}

void loop() {
    unsigned long currentTime = millis();
    
    // Проверка таймера игнорирования концевика
    if (_isLimitIgnored && (currentTime - _limitIgnoreStartTime) > LIMIT_IGNORE_TIME) {
        _isLimitIgnored = false;
        Serial.println(F("[MAIN] End ignore"));
    }

    if (_isLimitIgnoredTank10 && (currentTime - _limitIgnoreStartTimeTank10) > LIMIT_IGNORE_TIME) {
        _isLimitIgnoredTank10 = false;
        Serial.println(F("[MAIN] End ignore tank10"));
    }

    if (TEST_MODE) {
        // Тест кнопки и MOSFET для основного бака
        if (button.isPressed() && !mosfet.isOn()) {
            Serial.println(F("[MAIN] Button press"));
            mosfet.turnOn();
            delay(MOSFET_DELAY);
            limitSwitch.reset();
            _limitIgnoreStartTime = currentTime;
            _isLimitIgnored = true;
            Serial.println(F("[MAIN] Start ignore"));
        }

        // Тест концевика для основного бака
        if (!_isLimitIgnored && mosfet.isOn() && limitSwitch.isTriggered()) {
            Serial.println(F("[MAIN] Limit switch triggered"));
            Serial.print(F("[MAIN] MOSFET state before: "));
            Serial.println(mosfet.isOn());
            mosfet.turnOff();
            Serial.print(F("[MAIN] MOSFET state after: "));
            Serial.println(mosfet.isOn());
        }

        // Тест кнопки и MOSFET для tank10
        if (buttonTank10.isPressed() && !mosfetTank10.isOn()) {
            Serial.println(F("[MAIN] Button press tank10"));
            mosfetTank10.turnOn();
            delay(MOSFET_DELAY);
            limitSwitchTank10.reset();
            _limitIgnoreStartTimeTank10 = currentTime;
            _isLimitIgnoredTank10 = true;
            Serial.println(F("[MAIN] Start ignore tank10"));
        }

        // Тест концевика для tank10
        if (!_isLimitIgnoredTank10 && mosfetTank10.isOn() && limitSwitchTank10.isTriggered()) {
            Serial.println(F("[MAIN] Limit switch triggered tank10"));
            Serial.print(F("[MAIN] MOSFET tank10 state before: "));
            Serial.println(mosfetTank10.isOn());
            mosfetTank10.turnOff();
            Serial.print(F("[MAIN] MOSFET tank10 state after: "));
            Serial.println(mosfetTank10.isOn());
        }

        // Тест расписания с интервалом
        if ((currentTime - _lastScheduleCheck) >= SCHEDULE_CHECK_INTERVAL) {
            _lastScheduleCheck = currentTime;
            
            // Проверка основного бака
            if (scheduler.shouldActivate(1)) {
                Serial.println(F("[MAIN] Schedule main tank"));
                mosfet.turnOn();
                delay(MOSFET_DELAY);
                limitSwitch.reset();
                _limitIgnoreStartTime = currentTime;
                _isLimitIgnored = true;
                Serial.println(F("[MAIN] Start ignore"));
            }
            
            // Проверка tank10
            if (scheduler.shouldActivate(2)) {
                Serial.println(F("[MAIN] Schedule tank10"));
                mosfetTank10.turnOn();
                delay(MOSFET_DELAY);
                limitSwitchTank10.reset();
                _limitIgnoreStartTimeTank10 = currentTime;
                _isLimitIgnoredTank10 = true;
                Serial.println(F("[MAIN] Start ignore tank10"));
            }
        }
    } else {
        // Нормальный режим работы
        // Обработка основного бака
        if (button.isPressed() && !mosfet.isOn()) {
            Serial.println(F("[MAIN] Button press"));
            mosfet.turnOn();
            delay(MOSFET_DELAY);
            limitSwitch.reset();
            _limitIgnoreStartTime = currentTime;
            _isLimitIgnored = true;
            Serial.println(F("[MAIN] Start ignore"));
        }

        if (!_isLimitIgnored && mosfet.isOn() && limitSwitch.isTriggered()) {
            Serial.println(F("[MAIN] Limit switch triggered"));
            Serial.print(F("[MAIN] MOSFET state before: "));
            Serial.println(mosfet.isOn());
            mosfet.turnOff();
            Serial.print(F("[MAIN] MOSFET state after: "));
            Serial.println(mosfet.isOn());
        }

        // Обработка tank10
        if (buttonTank10.isPressed() && !mosfetTank10.isOn()) {
            Serial.println(F("[MAIN] Button press tank10"));
            mosfetTank10.turnOn();
            delay(MOSFET_DELAY);
            limitSwitchTank10.reset();
            _limitIgnoreStartTimeTank10 = currentTime;
            _isLimitIgnoredTank10 = true;
            Serial.println(F("[MAIN] Start ignore tank10"));
        }

        if (!_isLimitIgnoredTank10 && mosfetTank10.isOn() && limitSwitchTank10.isTriggered()) {
            Serial.println(F("[MAIN] Limit switch triggered tank10"));
            Serial.print(F("[MAIN] MOSFET tank10 state before: "));
            Serial.println(mosfetTank10.isOn());
            mosfetTank10.turnOff();
            Serial.print(F("[MAIN] MOSFET tank10 state after: "));
            Serial.println(mosfetTank10.isOn());
        }

        // Проверка расписания с интервалом
        if ((currentTime - _lastScheduleCheck) >= SCHEDULE_CHECK_INTERVAL) {
            _lastScheduleCheck = currentTime;
            
            // Проверка основного бака
            if (scheduler.shouldActivate(1)) {
                Serial.println(F("[MAIN] Schedule main tank"));
                mosfet.turnOn();
                delay(MOSFET_DELAY);
                limitSwitch.reset();
                _limitIgnoreStartTime = currentTime;
                _isLimitIgnored = true;
                Serial.println(F("[MAIN] Start ignore"));
            }
            
            // Проверка tank10
            if (scheduler.shouldActivate(2)) {
                Serial.println(F("[MAIN] Schedule tank10"));
                mosfetTank10.turnOn();
                delay(MOSFET_DELAY);
                limitSwitchTank10.reset();
                _limitIgnoreStartTimeTank10 = currentTime;
                _isLimitIgnoredTank10 = true;
                Serial.println(F("[MAIN] Start ignore tank10"));
            }
        }
    }
    delay(100);
}