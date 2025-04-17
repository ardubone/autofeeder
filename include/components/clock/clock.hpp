#pragma once

#include <RTClib.h>

class Clock {
private:
    RTC_DS1307 _rtc;

public:
    Clock() {}

    void init() {
        Serial.println("[CLOCK] Инициализация часов DS1307");
        if (!_rtc.begin()) {
            Serial.println("[CLOCK] Ошибка инициализации RTC");
            return;
        }
        
        if (!_rtc.isrunning()) {
            Serial.println("[CLOCK] RTC не запущен, устанавливаем время по умолчанию");
            _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        }
    }

    DateTime getTime() {
        return _rtc.now();
    }

    void setTime(const DateTime& dt) {
        Serial.print("[CLOCK] Установка времени: ");
        Serial.print(dt.hour());
        Serial.print(":");
        Serial.println(dt.minute());
        _rtc.adjust(dt);
    }

    uint8_t getDayOfWeek() {
        return _rtc.now().dayOfTheWeek();
    }
}; 