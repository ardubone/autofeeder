# Автоматическая кормушка для рыб 
### Данный пример можно использовать для любой авто кормушки, я взял самую дешевую электронную кормушку для рыб, которая срабатывала по 12/24 часа и с кнопки. Вынул электронику, мотор подключил через фет, при работе с ардуино нужно использовать понижайку до 3v, так как моя кормушка была от двух батареек.


## Подключение проводов

### 1. Часы DS1307
- VCC -> 5V Arduino
- GND -> GND Arduino
- SDA -> A4 (SDA) Arduino
- SCL -> A5 (SCL) Arduino

### 2. Кнопка
- Один контакт -> GND Arduino
- Другой контакт -> Пин 2 Arduino
- Подтягивающий резистор 10кОм между пином 2 и 5V

### 3. Концевик
- Один контакт -> GND Arduino
- Другой контакт -> Пин 3 Arduino
- Подтягивающий резистор 10кОм между пином 3 и 5V

### 4. MOSFET
- Gate -> Пин 4 Arduino
- Source -> GND Arduino
- Drain -> Минус двигателя
- Плюс двигателя -> 12V питания

### 5. Питание
- 5V Arduino -> Внешний источник 5V
- GND Arduino -> Общий GND
- 12V -> Внешний источник 12V для двигателя

## План тестирования

### 1. Подготовка
1. Подключите Arduino к компьютеру
2. Откройте монитор порта (9600 бод)
3. Загрузите код в Arduino

### 2. Тест компонентов по отдельности
1. Проверьте логи инициализации:
   - Должны появиться сообщения от всех компонентов
   - Проверьте сообщения об ошибках

2. Проверьте работу часов:
   - В логах должно быть текущее время
   - Проверьте установку времени через `clock.setTime()`
   - Проверьте получение времени через `clock.getTime()`
   - Проверьте день недели через `clock.getDayOfWeek()`

3. Проверьте работу кнопки:
   - Нажмите кнопку
   - В логах должно появиться сообщение о нажатии
   - MOSFET должен включиться

4. Проверьте работу концевика:
   - Активируйте концевик
   - В логах должно появиться сообщение о срабатывании
   - MOSFET должен выключиться

5. Проверьте работу MOSFET:
   - Включите через кнопку
   - Выключите через концевик
   - Проверьте состояние в логах

### 3. Тест интеграции
1. Нажмите кнопку:
   - MOSFET должен включиться
   - В логах должно быть сообщение о включении

2. Активируйте концевик:
   - MOSFET должен выключиться
   - В логах должно быть сообщение о выключении

3. Дождитесь времени по расписанию:
   - MOSFET должен включиться
   - В логах должно быть сообщение о срабатывании по расписанию

### 4. Тест расписания
1. Установите тестовое время:
   ```cpp
   DateTime testTime(2024, 1, 1, 12, 0, 0); // Понедельник, 12:00
   clock.setTime(testTime);
   ```

2. Добавьте тестовое расписание:
   ```cpp
   scheduler.addSchedule(12, 0, 1); // Понедельник, 12:00
   ```

3. Проверьте срабатывание:
   - MOSFET должен включиться в 12:00
   - В логах должно быть сообщение о срабатывании

### 5. Финальный тест
1. Отключите тестовый режим:
   ```cpp
   const bool TEST_MODE = false;
   ```

2. Установите реальное время:
   ```cpp
   DateTime realTime(2024, 1, 1, 12, 0, 0);
   clock.setTime(realTime);
   ```

3. Добавьте реальное расписание:
   ```cpp
   scheduler.addSchedule(12, 0, 1); // Понедельник, 12:00
   ```

4. Проверьте работу всей системы:
   - Проверьте работу кнопки
   - Проверьте работу концевика
   - Проверьте работу по расписанию
   - Проверьте логи

### 6. Проверка безопасности
1. Проверьте отключение по концевику:
   - Включите MOSFET
   - Активируйте концевик
   - MOSFET должен выключиться

2. Проверьте защиту от повторного включения:
   - Включите MOSFET
   - Попробуйте включить снова
   - MOSFET должен остаться включенным

### 7. Проверка питания
1. Проверьте работу от батарейки:
   - Отключите USB
   - Проверьте работу часов
   - Проверьте сохранение времени

2. Проверьте работу от USB:
   - Подключите USB
   - Проверьте работу всех компонентов

### 8. Проверка надежности
1. Проверьте работу после перезагрузки:
   - Перезагрузите Arduino
   - Проверьте сохранение времени
   - Проверьте сохранение расписания

2. Проверьте переход через полночь:
   - Установите время 23:59
   - Дождитесь 00:00
   - Проверьте работу расписания

3. Проверьте переход через неделю:
   - Установите время воскресенье 23:59
   - Дождитесь понедельника 00:00
   - Проверьте работу расписания 



#### Данный код полностью написан ИИ, так как мне нужен был быстрый рабочий MVP для другого проекта на плюсах. Промт с которого начал:
```
надо сделать проект на ардуино uno вот список компонентов и необходимых функций, делай сразу по бэм
компоненты:
часы ds1302
кнопка
механический концевик
модуль MOSFET 

отдельные Функции:
Реакция на кнопку
Реакция на концевик
Подача сигнала на модуль MOSFET 
Остановка сигнала на модуль MOSFET 
Функция работы по времени
Функция работы по дням недели

Интеграции:
Функция Подача сигнала на реле по Реакция на кнопку
Функция остановки Подача сигнала на реле по Реакция на концевик
Функция работы по определенному времени в определенный день недели, в день может быть несколько времен.

Финал:
Функция работы по времени Подача сигнала на реле по Реакция на кнопку и остановки Подача сигнала на реле по Реакция на концевик
```
Далее отладка и дополнение.