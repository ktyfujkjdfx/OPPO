#include "date_utils.h"

#include <iomanip>
#include <sstream>

bool IsValidDate(const std::string& date) {
    // Проверка длины формата ГГГГ.ММ.ДД
    if (date.length() != 10) return false;

    int year, month, day;
    char dot1, dot2;

    // Парсинг строки
    std::istringstream ss(date);
    ss >> year >> dot1 >> month >> dot2 >> day;

    // Проверка формата (должны быть точки)
    if (dot1 != '.' || dot2 != '.') return false;

    // Проверка диапазонов
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;

    // Количество дней в месяцах
    static const int days_in_month[] = { 31, 28, 31, 30, 31, 30,
                                        31, 31, 30, 31, 30, 31 };

    // Проверка високосного года
    bool is_leap_year =
        (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    int max_day = days_in_month[month - 1];
    if (month == 2 && is_leap_year) max_day = 29;

    // Проверка дня
    return day >= 1 && day <= max_day;
}