#include "blocks/scheduler/scheduler.hpp"

Scheduler::Scheduler() : _clock(nullptr), _scheduleCount(0), _mosfetCount(0), _nextScheduleId(1) {
    for (uint8_t i = 0; i < MAX_TANKS; i++) {
        _mosfets[i] = nullptr;
    }
}

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

void Scheduler::init(Clock* clock) {
    _clock = clock;
    
    Serial.println(F("[SCHEDULER] Инициализация планировщика"));
    printAllSchedules();
}

void Scheduler::addMosfet(Mosfet* mosfet) {
    if (_mosfetCount < MAX_TANKS) {
        _mosfets[_mosfetCount] = mosfet;
        _mosfetCount++;
    }
}

uint8_t Scheduler::addSchedule(uint8_t hour, uint8_t minute, uint8_t dayOfWeek, uint8_t tankId) {
    if (_scheduleCount < MAX_SCHEDULES) {
        uint8_t id = _nextScheduleId++;
        _schedules[_scheduleCount] = {hour, minute, dayOfWeek, tankId, id, true};
        _scheduleCount++;
        
        Serial.print(F("[SCHEDULER] Добавлено расписание - ID: "));
        Serial.print(id);
        Serial.print(F(", Бак: "));
        Serial.print(tankId);
        Serial.print(F(", Время: "));
        Serial.print(hour);
        Serial.print(F(":"));
        if (minute < 10) Serial.print(F("0"));
        Serial.print(minute);
        Serial.print(F(", День: "));
        Serial.println(getDayName(dayOfWeek));
        
        return id;
    }
    return 0; // Ошибка, не удалось добавить
}

int Scheduler::findScheduleIndexById(uint8_t id) {
    for (int i = 0; i < _scheduleCount; i++) {
        if (_schedules[i].id == id) {
            return i;
        }
    }
    return -1; // Не найдено
}

bool Scheduler::removeSchedule(uint8_t id) {
    int index = findScheduleIndexById(id);
    if (index == -1) {
        Serial.print(F("[SCHEDULER] Ошибка - не найдено расписание с ID: "));
        Serial.println(id);
        return false;
    }
    
    Serial.print(F("[SCHEDULER] Удаление расписания - ID: "));
    Serial.println(id);
    
    // Сдвигаем все элементы после удаляемого
    for (int i = index; i < _scheduleCount - 1; i++) {
        _schedules[i] = _schedules[i + 1];
    }
    
    _scheduleCount--;
    return true;
}

bool Scheduler::enableSchedule(uint8_t id, bool enable) {
    int index = findScheduleIndexById(id);
    if (index == -1) {
        Serial.print(F("[SCHEDULER] Ошибка - не найдено расписание с ID: "));
        Serial.println(id);
        return false;
    }
    
    _schedules[index].enabled = enable;
    
    Serial.print(F("[SCHEDULER] Расписание - ID: "));
    Serial.print(id);
    if (enable) {
        Serial.println(F(", Статус: Включено"));
    } else {
        Serial.println(F(", Статус: Отключено"));
    }
    
    return true;
}

bool Scheduler::updateSchedule(uint8_t id, uint8_t hour, uint8_t minute, uint8_t dayOfWeek, uint8_t tankId) {
    int index = findScheduleIndexById(id);
    if (index == -1) {
        Serial.print(F("[SCHEDULER] Ошибка - не найдено расписание с ID: "));
        Serial.println(id);
        return false;
    }
    
    _schedules[index].hour = hour;
    _schedules[index].minute = minute;
    _schedules[index].dayOfWeek = dayOfWeek;
    _schedules[index].tankId = tankId;
    
    Serial.print(F("[SCHEDULER] Обновлено расписание - ID: "));
    Serial.print(id);
    Serial.print(F(", Бак: "));
    Serial.print(tankId);
    Serial.print(F(", Время: "));
    Serial.print(hour);
    Serial.print(F(":"));
    if (minute < 10) Serial.print(F("0"));
    Serial.print(minute);
    Serial.print(F(", День: "));
    Serial.println(getDayName(dayOfWeek));
    
    return true;
}

void Scheduler::clearAllSchedules() {
    Serial.println(F("[SCHEDULER] Очистка всех расписаний"));
    _scheduleCount = 0;
}

uint8_t Scheduler::getScheduleCount() const {
    return _scheduleCount;
}

Schedule Scheduler::getSchedule(uint8_t index) const {
    if (index < _scheduleCount) {
        return _schedules[index];
    }
    // Возвращаем пустое расписание в случае ошибки
    return {0, 0, 0, 0, 0, false};
}

void Scheduler::printAllSchedules() {
    Serial.println(F("[SCHEDULER] Текущие расписания:"));
    
    if (_scheduleCount == 0) {
        Serial.println(F("[SCHEDULER] Нет активных расписаний"));
        return;
    }
    
    for (uint8_t i = 0; i < _scheduleCount; i++) {
        Serial.print(F("[SCHEDULER] ID: "));
        Serial.print(_schedules[i].id);
        Serial.print(F(", Статус: "));
        Serial.print(_schedules[i].enabled ? F("Включено") : F("Отключено"));
        Serial.print(F(", Бак: "));
        Serial.print(_schedules[i].tankId);
        Serial.print(F(", Время: "));
        Serial.print(_schedules[i].hour);
        Serial.print(F(":"));
        if (_schedules[i].minute < 10) Serial.print(F("0"));
        Serial.print(_schedules[i].minute);
        Serial.print(F(", День: "));
        Serial.println(getDayName(_schedules[i].dayOfWeek));
    }
}

Mosfet* Scheduler::getMosfet(uint8_t tankId) {
    for (uint8_t i = 0; i < _mosfetCount; i++) {
        if (_mosfets[i] && _mosfets[i]->getTankId() == tankId) {
            return _mosfets[i];
        }
    }
    return nullptr;
}

bool Scheduler::shouldActivate(uint8_t tankId) {
    if (!_clock) return false;

    Mosfet* mosfet = getMosfet(tankId);
    if (!mosfet) return false;

    DateTime now = _clock->getTime();
    uint8_t currentDay = now.dayOfTheWeek();
    uint8_t currentHour = now.hour();
    uint8_t currentMinute = now.minute();

    // Вывод расписаний для текущего дня
    Serial.println(F("[SCHEDULER] Расписания на сегодня:"));
    bool hasTodaySchedules = false;
    
    for (uint8_t i = 0; i < _scheduleCount; i++) {
        if (_schedules[i].dayOfWeek == currentDay && _schedules[i].enabled) {
            hasTodaySchedules = true;
            Serial.print(F("[SCHEDULER] ID: "));
            Serial.print(_schedules[i].id);
            Serial.print(F(", Бак: "));
            Serial.print(_schedules[i].tankId);
            Serial.print(F(", Время: "));
            Serial.print(_schedules[i].hour);
            Serial.print(F(":"));
            if (_schedules[i].minute < 10) Serial.print(F("0"));
            Serial.println(_schedules[i].minute);
        }
    }
    
    if (!hasTodaySchedules) {
        Serial.println(F("[SCHEDULER] Нет расписаний на сегодня"));
    }

    for (uint8_t i = 0; i < _scheduleCount; i++) {
        if (_schedules[i].tankId == tankId &&
            _schedules[i].dayOfWeek == currentDay &&
            _schedules[i].hour == currentHour &&
            _schedules[i].minute == currentMinute &&
            _schedules[i].enabled) {
            
            if (!mosfet->isOn()) {
                Serial.print(F("[SCHEDULER] Активация по расписанию - ID: "));
                Serial.println(_schedules[i].id);
                return true;
            }
        }
    }
    
    return false;
} 