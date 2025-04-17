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
const bool TEST_MODE = true;
const unsigned long MOSFET_DELAY = 1000; // Задержка после включения MOSFET в мс
const unsigned long LIMIT_IGNORE_TIME = 1500; // Время игнорирования концевика в мс

// Таймеры
unsigned long _limitIgnoreStartTime = 0;
bool _isLimitIgnored = false;

void setup() {
    Serial.begin(9600);
    Serial.println("[MAIN] Инициализация системы");
    
    clock.init();
    button.init();
    limitSwitch.init();
    mosfet.init();
    scheduler.init(&clock, &mosfet);
    
    // Добавляем тестовое расписание
    scheduler.addSchedule(12, 0, 1, 2); // Понедельник, 12:00, 2 повторение
    scheduler.addSchedule(12, 01, 1, 1); 
    scheduler.addSchedule(12, 02, 1, 3); 
    
    // Устанавливаем тестовое время
    DateTime testTime(2024, 1, 1, 12, 0, 0); // Понедельник, 12:00
    clock.setTime(testTime);
}

void loop() {
    // Проверка таймера игнорирования концевика
    if (_isLimitIgnored && (millis() - _limitIgnoreStartTime) > LIMIT_IGNORE_TIME) {
        _isLimitIgnored = false;
        Serial.println("[MAIN] Завершено игнорирование концевика");
    }

    if (TEST_MODE) {
        // Тест кнопки и MOSFET
        if (button.isPressed() && !mosfet.isOn()) {
            Serial.println("[MAIN] Нажата кнопка, включаем MOSFET");
            mosfet.turnOn();
            delay(MOSFET_DELAY); // Задержка после включения
            limitSwitch.reset();
            _limitIgnoreStartTime = millis();
            _isLimitIgnored = true;
            Serial.println("[MAIN] Начато игнорирование концевика");
        }

        // Тест концевика
        if (limitSwitch.isTriggered() && !_isLimitIgnored && mosfet.isOn()) {
            Serial.println("[MAIN] Сработал концевик, выключаем MOSFET");
            mosfet.turnOff();
        }

        // Тест расписания
        if (scheduler.shouldActivate() && !mosfet.isOn()) {
            Serial.println("[MAIN] Активация по расписанию, включаем MOSFET");
            mosfet.turnOn();
            delay(MOSFET_DELAY); // Задержка после включения
            limitSwitch.reset();
            _limitIgnoreStartTime = millis();
            _isLimitIgnored = true;
            Serial.println("[MAIN] Начато игнорирование концевика");
        }
    } else {
        // Нормальный режим работы
        if (button.isPressed() && !mosfet.isOn()) {
            Serial.println("[MAIN] Нажата кнопка, включаем MOSFET");
            mosfet.turnOn();
            delay(MOSFET_DELAY); // Задержка после включения
            limitSwitch.reset();
            _limitIgnoreStartTime = millis();
            _isLimitIgnored = true;
            Serial.println("[MAIN] Начато игнорирование концевика");
        }

        if (limitSwitch.isTriggered() && !_isLimitIgnored && mosfet.isOn()) {
            Serial.println("[MAIN] Сработал концевик, выключаем MOSFET");
            mosfet.turnOff();
        }

        if (scheduler.shouldActivate() && !mosfet.isOn()) {
            Serial.println("[MAIN] Активация по расписанию, включаем MOSFET");
            mosfet.turnOn();
            delay(MOSFET_DELAY); // Задержка после включения
            limitSwitch.reset();
            _limitIgnoreStartTime = millis();
            _isLimitIgnored = true;
            Serial.println("[MAIN] Начато игнорирование концевика");
        }
    }
    delay(100);
}