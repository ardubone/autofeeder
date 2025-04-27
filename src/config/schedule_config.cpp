#include "config/schedule_config.hpp"

// Инициализация глобальной переменной для хранения ID расписаний
ScheduleIds scheduleIds;

// Реализация функции для настройки расписания
void setupSchedule(Scheduler& scheduler) {
    // Очистка предыдущих расписаний
    scheduler.clearAllSchedules();
    
    Serial.println(F("[MAIN] Настройка расписания"));
    
    // Добавляем расписание для tank20 (id 1)
    // Понедельник
    scheduleIds.tank20_mon_morning = scheduler.addSchedule(9, 0, 1, 1);  // 9:00
    scheduleIds.tank20_mon_evening = scheduler.addSchedule(19, 0, 1, 1); // 19:00
    
    // Вторник
    scheduleIds.tank20_tue_morning = scheduler.addSchedule(9, 0, 2, 1);
    scheduleIds.tank20_tue_evening = scheduler.addSchedule(19, 0, 2, 1);
    
    // Среда
    scheduleIds.tank20_wed_morning = scheduler.addSchedule(9, 0, 3, 1);
    scheduleIds.tank20_wed_evening = scheduler.addSchedule(19, 0, 3, 1);
    
    // Четверг
    scheduleIds.tank20_thu_morning = scheduler.addSchedule(9, 0, 4, 1);
    scheduleIds.tank20_thu_evening = scheduler.addSchedule(19, 0, 4, 1);
    
    // Пятница
    scheduleIds.tank20_fri_morning = scheduler.addSchedule(9, 0, 5, 1);
    scheduleIds.tank20_fri_evening = scheduler.addSchedule(19, 0, 5, 1);
    
    // Воскресенье
    scheduleIds.tank20_sun_morning = scheduler.addSchedule(9, 0, 0, 1);
    scheduleIds.tank20_sun_evening = scheduler.addSchedule(19, 0, 0, 1);
    
    // Добавляем расписание для tank10 (id 2)
    // Расписание на каждый день недели
    scheduleIds.tank10_mon = scheduler.addSchedule(20, 0, 1, 2); // Понедельник
    scheduleIds.tank10_tue = scheduler.addSchedule(20, 0, 2, 2); // Вторник
    scheduleIds.tank10_wed = scheduler.addSchedule(20, 0, 3, 2); // Среда
    scheduleIds.tank10_thu = scheduler.addSchedule(20, 0, 4, 2); // Четверг
    scheduleIds.tank10_fri = scheduler.addSchedule(20, 0, 5, 2); // Пятница
    scheduleIds.tank10_sun = scheduler.addSchedule(20, 0, 0, 2); // Воскресенье
    
    // Примеры управления расписанием:
    // Отключение расписания на среду
    // scheduler.enableSchedule(scheduleIds.tank10_wed, false);
    
    // Изменение времени расписания
    // scheduler.updateSchedule(scheduleIds.tank10_sun, 18, 30, 0, 2); // Меняем воскресное расписание на 18:30
    
    // Выводим все расписания
    scheduler.printAllSchedules();
} 