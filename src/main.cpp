#include <Arduino.h>
#include "components/clock/clock.hpp"
#include "components/button/button.hpp"
#include "components/limit_switch/limit_switch.hpp"
#include "components/mosfet/mosfet.hpp"
#include "blocks/scheduler/scheduler.hpp"
#include "blocks/tank_controller/tank_controller.hpp"
#include "config/schedule_config.hpp"

// Константы
const unsigned long MOSFET_DELAY = 1000; // Задержка после включения MOSFET в мс
const unsigned long MOSFET_OFF_DELAY = 200; // Задержка после выключения MOSFET в мс
const unsigned long LIMIT_IGNORE_TIME = 5000; // Время игнорирования концевика в мс
const unsigned long SCHEDULE_CHECK_INTERVAL = 45000; // Интервал проверки расписания в мс
const unsigned long INIT_STABILIZATION_DELAY = 2000; // Задержка для стабилизации питания при запуске
const unsigned long POWER_FLUCTUATION_THRESHOLD = 200; // Порог времени для игнорирования повторных срабатываний
const unsigned long EMERGENCY_SCHEDULE_CHECK = 5000; // Интервал проверки расписания в аварийном режиме
const unsigned long MAX_UNSTABLE_TIME = 10000; // Максимальное время в нестабильном состоянии

// Инициализация компонентов
Clock clock;
Button buttonTank20(2, 1); // Пин 2 для кнопки tank20, tank id 1
LimitSwitch limitSwitchTank20(3, 1); // Пин 3 для концевика tank20, tank id 1
Mosfet mosfetTank20(4, 1); // Пин 4 для MOSFET tank20, tank id 1
Button buttonTank10(5, 2); // Пин 5 для кнопки tank10, tank id 2
LimitSwitch limitSwitchTank10(6, 2); // Пин 6 для концевика tank10, tank id 2
Mosfet mosfetTank10(7, 2); // Пин 7 для MOSFET tank10, tank id 2

// Контроллеры баков
TankController tankController20(1, &buttonTank20, &limitSwitchTank20, &mosfetTank20, LIMIT_IGNORE_TIME, MOSFET_DELAY);
TankController tankController10(2, &buttonTank10, &limitSwitchTank10, &mosfetTank10, LIMIT_IGNORE_TIME, MOSFET_DELAY);

// Планировщик
Scheduler scheduler;

// Таймеры и состояния
unsigned long _lastScheduleCheck = 0;
unsigned long _lastActivation20 = 0; // Время последней активации для бака 20
unsigned long _lastActivation10 = 0; // Время последней активации для бака 10
bool _systemStabilized = false; // Флаг стабилизации системы
unsigned long _systemStartTime = 0; // Время запуска системы
bool _emergencyMode = false; // Флаг аварийного режима

void setup() {
    Serial.begin(9600);
    Serial.println(F("[MAIN] Init"));
    
    _systemStartTime = millis();
    
    // Задержка для стабилизации питания
    delay(INIT_STABILIZATION_DELAY);
    
    // Инициализация компонентов
    clock.init();
    // buttonTank20.init(); // Ручное управление отключено
    limitSwitchTank20.init();
    mosfetTank20.init();
    // buttonTank10.init(); // Ручное управление отключено
    limitSwitchTank10.init();
    mosfetTank10.init();
    
    // Гарантированно выключаем MOSFET при запуске
    mosfetTank20.turnOff();
    mosfetTank10.turnOff();
    delay(MOSFET_OFF_DELAY);
    
    // Инициализация контроллеров баков
    tankController20.init();
    tankController10.init();
    
    // Инициализация планировщика
    scheduler.init(&clock);
    scheduler.addMosfet(&mosfetTank20);
    scheduler.addMosfet(&mosfetTank10);
    
    // Настройка расписания из выделенного файла конфигурации
    setupSchedule(scheduler);
    
    // Установка времени
    // DateTime currentTime(2025, 1, 1, 12, 0, 0);
    // clock.setTime(currentTime);
    
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
    
    // Стабилизация завершена
    _systemStabilized = true;
    Serial.println(F("[MAIN] Система стабилизирована и готова к работе"));
}

void loop() {
    unsigned long currentTime = millis();
    
    // Проверяем аварийный режим
    if (!_systemStabilized) {
        // Если система не стабилизировалась в течение MAX_UNSTABLE_TIME, переходим в аварийный режим
        if ((currentTime - _systemStartTime) > MAX_UNSTABLE_TIME && !_emergencyMode) {
            _emergencyMode = true;
            Serial.println(F("[MAIN] ВНИМАНИЕ: Переход в аварийный режим"));
        }
        
        // В аварийном режиме проверяем только расписание
        if (_emergencyMode && (currentTime - _lastScheduleCheck) >= EMERGENCY_SCHEDULE_CHECK) {
            _lastScheduleCheck = currentTime;
            
            Serial.println(F("[MAIN] Проверка расписания в аварийном режиме"));
            
            // Проверка tank20 в аварийном режиме
            if (scheduler.shouldActivate(1)) {
                if ((currentTime - _lastActivation20) > POWER_FLUCTUATION_THRESHOLD * 5) { // Увеличенный порог в аварийном режиме
                    tankController20.activateBySchedule();
                    _lastActivation20 = currentTime;
                }
            }
            
            // Проверка tank10 в аварийном режиме
            if (scheduler.shouldActivate(2)) {
                if ((currentTime - _lastActivation10) > POWER_FLUCTUATION_THRESHOLD * 5) { // Увеличенный порог в аварийном режиме
                    tankController10.activateBySchedule();
                    _lastActivation10 = currentTime;
                }
            }
        }
        
        delay(100);
        return;
    }
    
    // Сброс аварийного режима если система стабилизировалась
    if (_emergencyMode) {
        _emergencyMode = false;
        Serial.println(F("[MAIN] Выход из аварийного режима"));
    }
    
    // Обновление контроллеров баков
    tankController20.update(currentTime);
    tankController10.update(currentTime);

    // Проверка расписания с интервалом
    if ((currentTime - _lastScheduleCheck) >= SCHEDULE_CHECK_INTERVAL) {
        _lastScheduleCheck = currentTime;
        
        // Проверка tank20
        if (scheduler.shouldActivate(1)) {
            // Защита от частых срабатываний
            if ((currentTime - _lastActivation20) > POWER_FLUCTUATION_THRESHOLD) {
                tankController20.activateBySchedule();
                _lastActivation20 = currentTime;
            } else {
                Serial.println(F("[MAIN] Игнорирование слишком частой активации для бака 1"));
            }
        }
        
        // Проверка tank10
        if (scheduler.shouldActivate(2)) {
            // Защита от частых срабатываний
            if ((currentTime - _lastActivation10) > POWER_FLUCTUATION_THRESHOLD) {
                tankController10.activateBySchedule();
                _lastActivation10 = currentTime;
            } else {
                Serial.println(F("[MAIN] Игнорирование слишком частой активации для бака 2"));
            }
        }
    }
    
    delay(100);
}