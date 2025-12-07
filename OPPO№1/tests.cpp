//#define CATCH_CONFIG_MAIN
//#include "catch_amalgamated.hpp"
//#include <iostream>
//
//#include "RealEstate.h"
//#include "RealEstateParser.h"
//#include "RealEstateFilter.h"
//#include "RealEstateSorter.h"
//#include "date_utils.h"
//
//int main() {
//    setlocale(LC_ALL, "ru");
//    std::cout << "ЗАПУСК ТЕСТОВ ДЛЯ СИСТЕМЫ НЕДВИЖИМОСТИ" << std::endl;
//
//    Catch::Session session;
//    int result = session.run();
//
//    std::cout << "ТЕСТЫ ЗАВЕРШЕНЫ" << std::endl;
//    system("pause");
//    return result;
//}
//
//// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================
//
//std::vector<RealEstate> CreateTestProperties() {
//    std::vector<RealEstate> properties;
//
//    RealEstate obj1, obj2, obj3, obj4;
//
//    // Объект 1 - квартира
//    obj1.SetProperty("City Apartment");
//    obj1.SetOwner("Ivanov Ivan");
//    obj1.SetDate("2023.05.15");
//    obj1.SetPrice("250000");
//
//    // Объект 2 - загородный дом
//    obj2.SetProperty("Country House");
//    obj2.SetOwner("Petrova Maria");
//    obj2.SetDate("2022.12.01");
//    obj2.SetPrice("1800000");
//
//    // Объект 3 - офис
//    obj3.SetProperty("Office Space");
//    obj3.SetOwner("Sidorov Alex");
//    obj3.SetDate("2023.03.10");
//    obj3.SetPrice("500000");
//
//    // Объект 4 - с нулевой ценой (теперь будет полным, т.к. цена явно установлена)
//    obj4.SetProperty("Garage");
//    obj4.SetOwner("Kuznetsov Petr");
//    obj4.SetDate("2023.07.20");
//    obj4.SetPrice("0");
//
//    properties.push_back(obj1);
//    properties.push_back(obj2);
//    properties.push_back(obj3);
//    properties.push_back(obj4);
//
//    return properties;
//}
//
//// ==================== ТЕСТЫ ДЛЯ REALESTATE ====================
//
//TEST_CASE("RealEstate - Состояние по умолчанию", "[RealEstate][Basic]") {
//    RealEstate estate;
//    REQUIRE_FALSE(estate.IsComplete());
//    REQUIRE(estate.GetProperty().empty());
//    REQUIRE(estate.GetOwner().empty());
//    REQUIRE(estate.GetDate().empty());
//    REQUIRE(estate.GetPrice() == 0);
//}
//
//TEST_CASE("RealEstate - Установка и получение свойств",
//    "[RealEstate][GettersSetters]") {
//    RealEstate estate;
//
//    SECTION("Установка свойства") {
//        estate.SetProperty("Элитная квартира");
//        REQUIRE(estate.GetProperty() == "Элитная квартира");
//    }
//
//    SECTION("Установка владельца") {
//        estate.SetOwner("Петров Петр");
//        REQUIRE(estate.GetOwner() == "Петров Петр");
//    }
//
//    SECTION("Установка даты") {
//        estate.SetDate("2024.01.15");
//        REQUIRE(estate.GetDate() == "2024.01.15");
//    }
//
//    SECTION("Установка цены") {
//        estate.SetPrice("1500000");
//        REQUIRE(estate.GetPrice() == 1500000);
//        // Не полный - нет property, owner, date
//        REQUIRE(estate.IsComplete() == false);
//    }
//}
//
//TEST_CASE("RealEstate - Проверка полноты объекта",
//    "[RealEstate][Completeness]") {
//    RealEstate estate;
//
//    SECTION("Пустой объект") {
//        REQUIRE_FALSE(estate.IsComplete());
//    }
//
//    SECTION("Частично заполненный - не полный") {
//        estate.SetProperty("Квартира");
//        estate.SetOwner("Иванов");
//        // Нет даты и цена не установлена
//        REQUIRE_FALSE(estate.IsComplete());
//
//        estate.SetDate("2023.01.01");
//        // Цена не установлена
//        REQUIRE_FALSE(estate.IsComplete());
//    }
//
//    SECTION("Полностью заполненный с положительной ценой - полный") {
//        estate.SetProperty("Дом");
//        estate.SetOwner("Сидоров");
//        estate.SetDate("2023.06.15");
//        estate.SetPrice("2000000");
//        REQUIRE(estate.IsComplete());
//    }
//
//    SECTION("Полностью заполненный с нулевой ценой - полный") {
//        estate.SetProperty("Гараж");
//        estate.SetOwner("Петров");
//        estate.SetDate("2023.01.01");
//        estate.SetPrice("0");
//        // если цена явно установлена (даже как 0), объект полный
//        REQUIRE(estate.IsComplete());
//    }
//}
//
//TEST_CASE("RealEstate - Конвертация цены", "[RealEstate][Price]") {
//    RealEstate estate;
//
//    SECTION("Корректная цена") {
//        estate.SetPrice("150000");
//        REQUIRE(estate.GetPrice() == 150000);
//
//        estate.SetPrice("0");
//        REQUIRE(estate.GetPrice() == 0);
//
//        estate.SetPrice("9999999");
//        REQUIRE(estate.GetPrice() == 9999999);
//    }
//
//    SECTION("Некорректная цена") {
//        estate.SetPrice("не число");
//        REQUIRE(estate.GetPrice() == 0);
//        REQUIRE(estate.IsComplete() == false);
//
//        estate.SetPrice("");
//        REQUIRE(estate.GetPrice() == 0);
//
//        estate.SetPrice("123abc");
//        REQUIRE(estate.GetPrice() == 123);
//    }
//}
//
//TEST_CASE("RealEstate - Метод clear", "[RealEstate][Clear]") {
//    RealEstate estate;
//
//    estate.SetProperty("Тест");
//    estate.SetOwner("Тестовый Владелец");
//    estate.SetDate("2023.01.01");
//    estate.SetPrice("100000");
//    REQUIRE(estate.IsComplete());
//
//    estate.Clear();
//
//    REQUIRE(estate.GetProperty().empty());
//    REQUIRE(estate.GetOwner().empty());
//    REQUIRE(estate.GetDate().empty());
//    REQUIRE(estate.GetPrice() == 0);
//    REQUIRE_FALSE(estate.IsComplete());
//}
//// ========================= DATE =========================
//
//TEST_CASE("RealEstate - Валидация даты", "[RealEstate][Date]") {
//    RealEstate estate;
//
//    SECTION("Некорректные даты отклоняются") {
//        // Неправильные месяцы
//        estate.SetDate("2024.00.01");
//        REQUIRE(estate.GetDate().empty());
//
//        estate.SetDate("2024.13.01");
//        REQUIRE(estate.GetDate().empty());
//
//        estate.SetDate("2024.14.01");
//        REQUIRE(estate.GetDate().empty());
//
//        // Неправильные дни
//        estate.SetDate("2024.01.32");
//        REQUIRE(estate.GetDate().empty());
//
//        estate.SetDate("2024.02.30");
//        REQUIRE(estate.GetDate().empty());
//
//        estate.SetDate("2024.04.31");
//        REQUIRE(estate.GetDate().empty());
//
//        estate.SetDate("2024.07.89");
//        REQUIRE(estate.GetDate().empty());
//
//        estate.SetDate("2024.07.97");
//        REQUIRE(estate.GetDate().empty());
//
//        estate.SetDate("2024.07.984");
//        REQUIRE(estate.GetDate().empty());
//
//        // Неправильный формат
//        estate.SetDate("2024-07-20");
//        REQUIRE(estate.GetDate().empty());
//
//        estate.SetDate("2024.7.20");
//        REQUIRE(estate.GetDate().empty());
//
//        estate.SetDate("24.07.20");
//        REQUIRE(estate.GetDate().empty());
//
//        estate.SetDate("");
//        REQUIRE(estate.GetDate().empty());
//    }
//
//    SECTION("Корректные даты принимаются") {
//        estate.SetDate("2024.07.20");
//        REQUIRE(estate.GetDate() == "2024.07.20");
//
//        estate.SetDate("2024.02.29");  // високосный
//        REQUIRE(estate.GetDate() == "2024.02.29");
//
//        estate.SetDate("2023.02.28");  // не високосный
//        REQUIRE(estate.GetDate() == "2023.02.28");
//
//        estate.SetDate("2024.01.31");
//        REQUIRE(estate.GetDate() == "2024.01.31");
//
//        estate.SetDate("2024.04.30");
//        REQUIRE(estate.GetDate() == "2024.04.30");
//    }
//
//    SECTION("Объект с некорректной датой неполный") {
//        estate.SetProperty("Квартира");
//        estate.SetOwner("Иванов");
//        estate.SetPrice("100000");
//        estate.SetDate("2024.14.01");
//
//        REQUIRE_FALSE(estate.IsComplete());
//        REQUIRE(estate.GetDate().empty());
//    }
//}
//// ==================== ТЕСТЫ ДЛЯ REALESTATEPARSER ====================
//
//TEST_CASE("RealEstateParser - Парсинг различных случаев",
//    "[Parser][Parsing]") {
//    RealEstateParser parser;
//
//    SECTION("Пустая строка") {
//        auto result = parser.ParseFromString("");
//        REQUIRE(result.empty());
//    }
//
//    SECTION("Случайный текст") {
//        auto result = parser.ParseFromString("Это просто какой-то текст");
//        REQUIRE(result.empty());
//    }
//
//    SECTION("Корректный один объект") {
//        auto result = parser.ParseFromString(
//            "Object #1: Городская квартира. \"Иванов Иван Иванович\" "
//            "2023.05.15 price: 250000");
//
//        REQUIRE(result.size() == 1);
//        REQUIRE(result[0].GetProperty() == "Городская квартира");
//        REQUIRE(result[0].GetOwner() == "Иванов Иван Иванович");
//        REQUIRE(result[0].GetDate() == "2023.05.15");
//        REQUIRE(result[0].GetPrice() == 250000);
//        REQUIRE(result[0].IsComplete());
//    }
//
//    SECTION("Несколько объектов") {
//        auto result = parser.ParseFromString(
//            "Object #1: Квартира. \"Иванов\" 2023.01.01 price: 100000\n"
//            "OBJECT 2: Дом. \"Петров\" 2023.02.01 cost: 2000000\n"
//            "Object #3: Офис. \"Сидоров\" 2023.03.01 стоимость: 500000");
//
//        REQUIRE(result.size() == 3);
//        REQUIRE(result[0].GetProperty() == "Квартира");
//        REQUIRE(result[1].GetProperty() == "Дом");
//        REQUIRE(result[2].GetProperty() == "Офис");
//
//        REQUIRE(result[0].GetPrice() == 100000);
//        REQUIRE(result[1].GetPrice() == 2000000);
//        REQUIRE(result[2].GetPrice() == 500000);
//    }
//}
//
//TEST_CASE("RealEstateParser - Отсеивание неполных объектов",
//    "[Parser][Filtering]") {
//    RealEstateParser parser;
//
//    SECTION("Нет владельца") {
//        auto result = parser.ParseFromString(
//            "Object #1: Квартира. 2023.01.01 price: 100000");
//        REQUIRE(result.empty());
//    }
//
//    SECTION("Нет даты") {
//        auto result = parser.ParseFromString(
//            "Object #1: Квартира. \"Владелец\" price: 100000");
//        REQUIRE(result.empty());
//    }
//
//    SECTION("Нет цены") {
//        auto result = parser.ParseFromString(
//            "Object #1: Квартира. \"Владелец\" 2023.01.01");
//        REQUIRE(result.empty());
//    }
//
//    SECTION("Только описание") {
//        auto result = parser.ParseFromString(
//            "Object #1: Только описание объекта.");
//        REQUIRE(result.empty());
//    }
//}
//
//TEST_CASE("RealEstateParser - Обработка специальных символов",
//    "[Parser][Special]") {
//    RealEstateParser parser;
//
//    SECTION("Разные пробелы") {
//        auto result = parser.ParseFromString(
//            "  Object   #1  :  Тест  .  \"Владелец\"  2023.01.01  "
//            "price  :  100000  ");
//        REQUIRE(result.size() == 1);
//        REQUIRE(result[0].GetProperty() == "Тест");
//        REQUIRE(result[0].GetPrice() == 100000);
//    }
//}
//
//// ==================== ТЕСТЫ ДЛЯ REALESTATEFILTER ====================
//
//TEST_CASE("RealEstateFilter - Фильтрация по цене", "[Filter][Price]") {
//    auto properties = CreateTestProperties();
//
//    SECTION("Диапазон без объектов") {
//        auto result = RealEstateFilter::FilterByPriceRange(properties,
//            3000000, 4000000);
//        REQUIRE(result.empty());
//    }
//
//    SECTION("Диапазон с нулевой ценой") {
//        auto result = RealEstateFilter::FilterByPriceRange(properties, 0, 100000);
//        REQUIRE(result.size() == 1);  // объект с ценой 0
//        REQUIRE(result[0].GetPrice() == 0);
//    }
//}
//
//TEST_CASE("RealEstateFilter - Граничные случаи", "[Filter][EdgeCases]") {
//    auto properties = CreateTestProperties();
//
//    SECTION("Пустой список") {
//        std::vector<RealEstate> emptyList;
//        auto result = RealEstateFilter::FilterByPriceRange(emptyList, 0,
//            100000);
//        REQUIRE(result.empty());
//    }
//
//    SECTION("Неверный диапазон (мин > макс)") {
//        auto result = RealEstateFilter::FilterByPriceRange(properties,
//            500000, 100000);
//        REQUIRE(result.empty());
//    }
//
//    SECTION("Отрицательный диапазон") {
//        auto result = RealEstateFilter::FilterByPriceRange(properties,
//            -100000, -50000);
//        REQUIRE(result.empty());
//    }
//
//    SECTION("Очень большой диапазон") {
//        auto result = RealEstateFilter::FilterByPriceRange(properties,
//            0, 10000000);
//        REQUIRE(result.size() == properties.size());
//    }
//}
//
//// ==================== ТЕСТЫ ДЛЯ REALESTATESORTER ====================
//
//TEST_CASE("RealEstateSorter - Сортировка по дате", "[Sorter][Date]") {
//    auto properties = CreateTestProperties();
//
//    SECTION("Сортировка по возрастанию даты") {
//        RealEstateSorter::SortByDateAscending(properties);
//
//        REQUIRE(properties[0].GetDate() == "2022.12.01");
//        REQUIRE(properties[1].GetDate() == "2023.03.10");
//        REQUIRE(properties[2].GetDate() == "2023.05.15");
//        REQUIRE(properties[3].GetDate() == "2023.07.20");
//    }
//
//    SECTION("Сортировка по убыванию даты") {
//        RealEstateSorter::SortByDateDescending(properties);
//
//        REQUIRE(properties[0].GetDate() == "2023.07.20");
//        REQUIRE(properties[1].GetDate() == "2023.05.15");
//        REQUIRE(properties[2].GetDate() == "2023.03.10");
//        REQUIRE(properties[3].GetDate() == "2022.12.01");
//    }
//}
//
//TEST_CASE("RealEstateSorter - Особые случаи", "[Sorter][Special]") {
//    SECTION("Пустой список") {
//        std::vector<RealEstate> emptyList;
//        REQUIRE_NOTHROW(RealEstateSorter::SortByDateAscending(emptyList));
//        REQUIRE(emptyList.empty());
//    }
//
//    SECTION("Один элемент") {
//        std::vector<RealEstate> singleItem;
//        RealEstate obj;
//        obj.SetProperty("Тест");
//        obj.SetOwner("Владелец");
//        obj.SetDate("2023.01.01");
//        obj.SetPrice("100000");
//        singleItem.push_back(obj);
//
//        RealEstateSorter::SortByDateAscending(singleItem);
//        REQUIRE(singleItem.size() == 1);
//        REQUIRE(singleItem[0].GetDate() == "2023.01.01");
//    }
//}
//
//// ==================== ИНТЕГРАЦИОННЫЕ ТЕСТЫ ====================
//
//TEST_CASE("Интеграционный тест - Полный цикл обработки", "[Integration]") {
//    RealEstateParser parser;
//
//    std::string input =
//        "Object #1: Городская квартира. \"Иванов Иван\" 2023.05.15 "
//        "price: 250000\n"
//        "OBJECT 2: Загородный дом. \"Петрова Мария\" 2022.12.01 "
//        "cost: 1800000\n"
//        "Object #3: Офисное помещение. \"Сидоров Алексей\" 2023.03.10 "
//        "стоимость: 500000\n"
//        "Object #4: Неполный объект. только описание";
//
//    auto properties = parser.ParseFromString(input);
//    REQUIRE(properties.size() == 3);
//
//    auto filtered = RealEstateFilter::FilterByPriceRange(properties,
//        200000, 600000);
//    REQUIRE(filtered.size() == 2);
//
//    RealEstateSorter::SortByDateAscending(filtered);
//    REQUIRE(filtered[0].GetDate() == "2023.03.10");
//    REQUIRE(filtered[1].GetDate() == "2023.05.15");
//
//    for (const auto& estate : filtered) {
//        REQUIRE(estate.IsComplete());
//        REQUIRE(estate.GetPrice() >= 200000);
//        REQUIRE(estate.GetPrice() <= 600000);
//    }
//}
//
//// ==================== ТЕСТЫ ДЛЯ DATE_UTILS ====================
//
//TEST_CASE("DateUtils - Валидация дат", "[DateUtils][Validation]") {
//    SECTION("Корректные даты") {
//        REQUIRE(IsValidDate("2023.01.01") == true);
//        REQUIRE(IsValidDate("2024.02.29") == true);   // високосный год
//        REQUIRE(IsValidDate("2023.02.28") == true);   // не високосный
//        REQUIRE(IsValidDate("2023.12.31") == true);
//        REQUIRE(IsValidDate("2000.01.01") == true);   // граничные значения
//        REQUIRE(IsValidDate("2100.12.31") == true);
//    }
//
//    SECTION("Некорректные даты") {
//        REQUIRE(IsValidDate("") == false);
//        REQUIRE(IsValidDate("2023.01.32") == false);
//        REQUIRE(IsValidDate("2023.13.01") == false);
//        REQUIRE(IsValidDate("2023.00.01") == false);
//        REQUIRE(IsValidDate("2023.01.00") == false);
//        // неправильный формат
//        REQUIRE(IsValidDate("2023-01-01") == false);
//        // короткий год
//        REQUIRE(IsValidDate("23.01.01") == false);
//        // короткие месяц и день
//        REQUIRE(IsValidDate("2023.1.1") == false);
//        // год меньше 1900
//        REQUIRE(IsValidDate("1899.12.31") == false);
//        // год больше 2100
//        REQUIRE(IsValidDate("2101.01.01") == false);
//        // 29 февраля в невисокосный
//        REQUIRE(IsValidDate("2023.02.29") == false);
//    }
//}