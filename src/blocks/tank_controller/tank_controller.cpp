#include "blocks/tank_controller/tank_controller.hpp"

TankController::TankController(uint8_t tankId, Button* button, LimitSwitch* limitSwitch, Mosfet* mosfet,
                             unsigned long limitIgnoreTime, unsigned long mosfetDelay)
    : _tankId(tankId), 
      _button(button), 
      _limitSwitch(limitSwitch), 
      _mosfet(mosfet),
      _limitIgnoreStartTime(0),
      _isLimitIgnored(false),
      _limitIgnoreTime(limitIgnoreTime),
      _mosfetDelay(mosfetDelay) {}

void TankController::init() {
    Serial.print(F("[TANK_CONTROLLER] Инициализация контроллера для бака "));
    Serial.println(_tankId);
}

void TankController::update(unsigned long currentTime) {
    // Проверка таймера игнорирования концевика
    if (_isLimitIgnored && (currentTime - _limitIgnoreStartTime) > _limitIgnoreTime) {
        _isLimitIgnored = false;
        Serial.print(F("[TANK_CONTROLLER] Конец игнорирования концевика для бака "));
        Serial.println(_tankId);
    }

    // Проверка нажатия кнопки
    // if (_button->isPressed() && !_mosfet->isOn()) {
    //     activateByButton();
    // }

    // Проверка срабатывания концевика
    if (!_isLimitIgnored && _mosfet->isOn() && _limitSwitch->isTriggered()) {
        Serial.print(F("[TANK_CONTROLLER] Сработал концевик для бака "));
        Serial.println(_tankId);
        Serial.print(F("[TANK_CONTROLLER] MOSFET состояние до: "));
        Serial.println(_mosfet->isOn());
        _mosfet->turnOff();
        Serial.print(F("[TANK_CONTROLLER] MOSFET состояние после: "));
        Serial.println(_mosfet->isOn());
    }
}

void TankController::activateByButton() {
    // Метод сохранен для совместимости, но ручное управление отключено
    Serial.print(F("[TANK_CONTROLLER] Нажата кнопка для бака "));
    Serial.println(_tankId);
    _mosfet->turnOn();
    delay(_mosfetDelay);
    _limitSwitch->reset();
    _limitIgnoreStartTime = millis();
    _isLimitIgnored = true;
    Serial.print(F("[TANK_CONTROLLER] Начало игнорирования для бака "));
    Serial.println(_tankId);
}

void TankController::activateBySchedule() {
    Serial.print(F("[TANK_CONTROLLER] Активация по расписанию для бака "));
    Serial.println(_tankId);
    _mosfet->turnOn();
    delay(_mosfetDelay);
    _limitSwitch->reset();
    _limitIgnoreStartTime = millis();
    _isLimitIgnored = true;
    Serial.print(F("[TANK_CONTROLLER] Начало игнорирования для бака "));
    Serial.println(_tankId);
}

bool TankController::isLimitIgnored() const {
    return _isLimitIgnored;
}

uint8_t TankController::getTankId() const {
    return _tankId;
}

Mosfet* TankController::getMosfet() const {
    return _mosfet;
} 