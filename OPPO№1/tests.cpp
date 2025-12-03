#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include <iostream>
#include "RealEstate.h"
#include "RealEstateParser.h"
#include "RealEstateFilter.h"
#include "RealEstateSorter.h"


int main() {
    setlocale(LC_ALL, "ru");
    std::cout << "ЗАПУСК ТЕСТОВ ДЛЯ СИСТЕМЫ НЕДВИЖИМОСТИ" << std::endl;

    Catch::Session session;
    int result = session.run();

    std::cout << "ТЕСТЫ ЗАВЕРШЕНЫ" << std::endl;
    system("pause");
    return result;
}

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

std::vector<RealEstate> CreateTestProperties() {
    std::vector<RealEstate> properties;

    RealEstate obj1, obj2, obj3, obj4;

    // Объект 1 - квартира
    obj1.setProperty("City Apartment");
    obj1.setOwner("Ivanov Ivan");
    obj1.setDate("2023.05.15");
    obj1.setPrice("250000");

    // Объект 2 - загородный дом  
    obj2.setProperty("Country House");
    obj2.setOwner("Petrova Maria");
    obj2.setDate("2022.12.01");
    obj2.setPrice("1800000");

    // Объект 3 - офис
    obj3.setProperty("Office Space");
    obj3.setOwner("Sidorov Alex");
    obj3.setDate("2023.03.10");
    obj3.setPrice("500000");

    // Объект 4 - с нулевой ценой (теперь будет полным, т.к. цена явно установлена)
    obj4.setProperty("Garage");
    obj4.setOwner("Kuznetsov Petr");
    obj4.setDate("2023.07.20");
    obj4.setPrice("0");

    properties.push_back(obj1);
    properties.push_back(obj2);
    properties.push_back(obj3);
    properties.push_back(obj4);

    return properties;
}

// ==================== ТЕСТЫ ДЛЯ REALESTATE ====================

TEST_CASE("RealEstate - Состояние по умолчанию", "[RealEstate][Basic]") {
    RealEstate estate;
    REQUIRE_FALSE(estate.isComplete());
    REQUIRE(estate.getProperty().empty());
    REQUIRE(estate.getOwner().empty());
    REQUIRE(estate.getDate().empty());
    REQUIRE(estate.getPrice() == 0);
}

TEST_CASE("RealEstate - Установка и получение свойств", "[RealEstate][GettersSetters]") {
    RealEstate estate;

    SECTION("Установка свойства") {
        estate.setProperty("Элитная квартира");
        REQUIRE(estate.getProperty() == "Элитная квартира");
    }

    SECTION("Установка владельца") {
        estate.setOwner("Петров Петр");
        REQUIRE(estate.getOwner() == "Петров Петр");
    }

    SECTION("Установка даты") {
        estate.setDate("2024.01.15");
        REQUIRE(estate.getDate() == "2024.01.15");
    }

    SECTION("Установка цены") {
        estate.setPrice("1500000");
        REQUIRE(estate.getPrice() == 1500000);
        REQUIRE(estate.isComplete() == false); // Не полный - нет property, owner, date
    }
}

TEST_CASE("RealEstate - Проверка полноты объекта", "[RealEstate][Completeness]") {
    RealEstate estate;

    SECTION("Пустой объект") {
        REQUIRE_FALSE(estate.isComplete());
    }

    SECTION("Частично заполненный - не полный") {
        estate.setProperty("Квартира");
        estate.setOwner("Иванов");
        REQUIRE_FALSE(estate.isComplete()); // Нет даты и цена не установлена

        estate.setDate("2023.01.01");
        REQUIRE_FALSE(estate.isComplete()); // Цена не установлена
    }

    SECTION("Полностью заполненный с положительной ценой - полный") {
        estate.setProperty("Дом");
        estate.setOwner("Сидоров");
        estate.setDate("2023.06.15");
        estate.setPrice("2000000");
        REQUIRE(estate.isComplete());
    }

    SECTION("Полностью заполненный с нулевой ценой - полный") {
        estate.setProperty("Гараж");
        estate.setOwner("Петров");
        estate.setDate("2023.01.01");
        estate.setPrice("0");
        // если цена явно установлена (даже как 0), объект полный
        REQUIRE(estate.isComplete());
    }
}

TEST_CASE("RealEstate - Конвертация цены", "[RealEstate][Price]") {
    RealEstate estate;

    SECTION("Корректная цена") {
        estate.setPrice("150000");
        REQUIRE(estate.getPrice() == 150000);

        estate.setPrice("0");
        REQUIRE(estate.getPrice() == 0);

        estate.setPrice("9999999");
        REQUIRE(estate.getPrice() == 9999999);
    }

    SECTION("Некорректная цена") {
        estate.setPrice("не число");
        REQUIRE(estate.getPrice() == 0);
        REQUIRE(estate.isComplete() == false); 

        estate.setPrice("");
        REQUIRE(estate.getPrice() == 0);

        estate.setPrice("123abc");
        REQUIRE(estate.getPrice() == 123);
    }
}

TEST_CASE("RealEstate - Метод clear", "[RealEstate][Clear]") {
    RealEstate estate;

    estate.setProperty("Тест");
    estate.setOwner("Тестовый Владелец");
    estate.setDate("2023.01.01");
    estate.setPrice("100000");
    REQUIRE(estate.isComplete());

    estate.clear();

    REQUIRE(estate.getProperty().empty());
    REQUIRE(estate.getOwner().empty());
    REQUIRE(estate.getDate().empty());
    REQUIRE(estate.getPrice() == 0);
    REQUIRE_FALSE(estate.isComplete()); 
}
// ========================= DATE =========================

TEST_CASE("RealEstate - Валидация даты", "[RealEstate][Date]") {
    RealEstate estate;

    SECTION("Некорректные даты отклоняются") {
        // Неправильные месяцы
        estate.setDate("2024.00.01");
        REQUIRE(estate.getDate().empty());

        estate.setDate("2024.13.01");
        REQUIRE(estate.getDate().empty());

        estate.setDate("2024.14.01");
        REQUIRE(estate.getDate().empty());

        // Неправильные дни
        estate.setDate("2024.01.32");
        REQUIRE(estate.getDate().empty());

        estate.setDate("2024.02.30");
        REQUIRE(estate.getDate().empty());

        estate.setDate("2024.04.31");
        REQUIRE(estate.getDate().empty());

        estate.setDate("2024.07.89");
        REQUIRE(estate.getDate().empty());

        estate.setDate("2024.07.97");
        REQUIRE(estate.getDate().empty());

        estate.setDate("2024.07.984");
        REQUIRE(estate.getDate().empty());

        // Неправильный формат
        estate.setDate("2024-07-20");
        REQUIRE(estate.getDate().empty());

        estate.setDate("2024.7.20");
        REQUIRE(estate.getDate().empty());

        estate.setDate("24.07.20");
        REQUIRE(estate.getDate().empty());

        estate.setDate("");
        REQUIRE(estate.getDate().empty());
    }

    SECTION("Корректные даты принимаются") {
        estate.setDate("2024.07.20");
        REQUIRE(estate.getDate() == "2024.07.20");

        estate.setDate("2024.02.29"); // високосный
        REQUIRE(estate.getDate() == "2024.02.29");

        estate.setDate("2023.02.28"); // не високосный
        REQUIRE(estate.getDate() == "2023.02.28");

        estate.setDate("2024.01.31");
        REQUIRE(estate.getDate() == "2024.01.31");

        estate.setDate("2024.04.30");
        REQUIRE(estate.getDate() == "2024.04.30");
    }

    SECTION("Объект с некорректной датой неполный") {
        estate.setProperty("Квартира");
        estate.setOwner("Иванов");
        estate.setPrice("100000");
        estate.setDate("2024.14.01");

        REQUIRE_FALSE(estate.isComplete());
        REQUIRE(estate.getDate().empty());
    }
}
// ==================== ТЕСТЫ ДЛЯ REALESTATEPARSER ====================

TEST_CASE("RealEstateParser - Парсинг различных случаев", "[Parser][Parsing]") {
    RealEstateParser parser;

    SECTION("Пустая строка") {
        auto result = parser.parseFromString("");
        REQUIRE(result.empty());
    }

    SECTION("Случайный текст") {
        auto result = parser.parseFromString("Это просто какой-то текст");
        REQUIRE(result.empty());
    }

    SECTION("Корректный один объект") {
        auto result = parser.parseFromString(
            "Object #1: Городская квартира. \"Иванов Иван Иванович\" 2023.05.15 price: 250000"
        );

        REQUIRE(result.size() == 1);
        REQUIRE(result[0].getProperty() == "Городская квартира");
        REQUIRE(result[0].getOwner() == "Иванов Иван Иванович");
        REQUIRE(result[0].getDate() == "2023.05.15");
        REQUIRE(result[0].getPrice() == 250000);
        REQUIRE(result[0].isComplete());
    }

    SECTION("Несколько объектов") {
        auto result = parser.parseFromString(
            "Object #1: Квартира. \"Иванов\" 2023.01.01 price: 100000\n"
            "OBJECT 2: Дом. \"Петров\" 2023.02.01 cost: 2000000\n"
            "Object #3: Офис. \"Сидоров\" 2023.03.01 стоимость: 500000"
        );

        REQUIRE(result.size() == 3);
        REQUIRE(result[0].getProperty() == "Квартира");
        REQUIRE(result[1].getProperty() == "Дом");
        REQUIRE(result[2].getProperty() == "Офис");

        REQUIRE(result[0].getPrice() == 100000);
        REQUIRE(result[1].getPrice() == 2000000);
        REQUIRE(result[2].getPrice() == 500000);
    }
}

TEST_CASE("RealEstateParser - Отсеивание неполных объектов", "[Parser][Filtering]") {
    RealEstateParser parser;

    SECTION("Нет владельца") {
        auto result = parser.parseFromString("Object #1: Квартира. 2023.01.01 price: 100000");
        REQUIRE(result.empty());
    }

    SECTION("Нет даты") {
        auto result = parser.parseFromString("Object #1: Квартира. \"Владелец\" price: 100000");
        REQUIRE(result.empty());
    }

    SECTION("Нет цены") {
        auto result = parser.parseFromString("Object #1: Квартира. \"Владелец\" 2023.01.01");
    }

    SECTION("Только описание") {
        auto result = parser.parseFromString("Object #1: Только описание объекта.");
        REQUIRE(result.empty());
    }
}

TEST_CASE("RealEstateParser - Обработка специальных символов", "[Parser][Special]") {
    RealEstateParser parser;

    SECTION("Разные пробелы") {
        auto result = parser.parseFromString("  Object   #1  :  Тест  .  \"Владелец\"  2023.01.01  price  :  100000  ");
        REQUIRE(result.size() == 1);
        REQUIRE(result[0].getProperty() == "Тест");
        REQUIRE(result[0].getPrice() == 100000);
    }
}

// ==================== ТЕСТЫ ДЛЯ REALESTATEFILTER ====================

TEST_CASE("RealEstateFilter - Фильтрация по цене", "[Filter][Price]") {
    auto properties = CreateTestProperties();

    SECTION("Диапазон без объектов") {
        auto result = RealEstateFilter::filterByPriceRange(properties, 3000000, 4000000);
        REQUIRE(result.empty());
    }

    SECTION("Диапазон с нулевой ценой") {
        auto result = RealEstateFilter::filterByPriceRange(properties, 0, 100000);
        REQUIRE(result.size() == 1); // объект с ценой 0
        REQUIRE(result[0].getPrice() == 0);
    }
}

TEST_CASE("RealEstateFilter - Граничные случаи", "[Filter][EdgeCases]") {
    auto properties = CreateTestProperties();

    SECTION("Пустой список") {
        std::vector<RealEstate> emptyList;
        auto result = RealEstateFilter::filterByPriceRange(emptyList, 0, 100000);
        REQUIRE(result.empty());
    }

    SECTION("Неверный диапазон (мин > макс)") {
        auto result = RealEstateFilter::filterByPriceRange(properties, 500000, 100000);
        REQUIRE(result.empty());
    }

    SECTION("Отрицательный диапазон") {
        auto result = RealEstateFilter::filterByPriceRange(properties, -100000, -50000);
        REQUIRE(result.empty());
    }

    SECTION("Очень большой диапазон") {
        auto result = RealEstateFilter::filterByPriceRange(properties, 0, 10000000);
        REQUIRE(result.size() == properties.size());
    }
}

// ==================== ТЕСТЫ ДЛЯ REALESTATESORTER ====================

TEST_CASE("RealEstateSorter - Сортировка по дате", "[Sorter][Date]") {
    auto properties = CreateTestProperties();

    SECTION("Сортировка по возрастанию даты") {
        RealEstateSorter::sortByDateAscending(properties);

        REQUIRE(properties[0].getDate() == "2022.12.01");
        REQUIRE(properties[1].getDate() == "2023.03.10");
        REQUIRE(properties[2].getDate() == "2023.05.15");
        REQUIRE(properties[3].getDate() == "2023.07.20");
    }

    SECTION("Сортировка по убыванию даты") {
        RealEstateSorter::sortByDateDescending(properties);

        REQUIRE(properties[0].getDate() == "2023.07.20");
        REQUIRE(properties[1].getDate() == "2023.05.15");
        REQUIRE(properties[2].getDate() == "2023.03.10");
        REQUIRE(properties[3].getDate() == "2022.12.01");
    }
}

TEST_CASE("RealEstateSorter - Особые случаи", "[Sorter][Special]") {
    SECTION("Пустой список") {
        std::vector<RealEstate> emptyList;
        REQUIRE_NOTHROW(RealEstateSorter::sortByDateAscending(emptyList));
        REQUIRE(emptyList.empty());
    }

    SECTION("Один элемент") {
        std::vector<RealEstate> singleItem;
        RealEstate obj;
        obj.setProperty("Тест");
        obj.setOwner("Владелец");
        obj.setDate("2023.01.01");
        obj.setPrice("100000");
        singleItem.push_back(obj);

        RealEstateSorter::sortByDateAscending(singleItem);
        REQUIRE(singleItem.size() == 1);
        REQUIRE(singleItem[0].getDate() == "2023.01.01");
    }
}

// ==================== ИНТЕГРАЦИОННЫЕ ТЕСТЫ ====================

TEST_CASE("Интеграционный тест - Полный цикл обработки", "[Integration]") {
    RealEstateParser parser;

    std::string input =
        "Object #1: Городская квартира. \"Иванов Иван\" 2023.05.15 price: 250000\n"
        "OBJECT 2: Загородный дом. \"Петрова Мария\" 2022.12.01 cost: 1800000\n"
        "Object #3: Офисное помещение. \"Сидоров Алексей\" 2023.03.10 стоимость: 500000\n"
        "Object #4: Неполный объект. только описание";

    auto properties = parser.parseFromString(input);
    REQUIRE(properties.size() == 3);

    auto filtered = RealEstateFilter::filterByPriceRange(properties, 200000, 600000);
    REQUIRE(filtered.size() == 2);

    RealEstateSorter::sortByDateAscending(filtered);
    REQUIRE(filtered[0].getDate() == "2023.03.10");
    REQUIRE(filtered[1].getDate() == "2023.05.15");

    for (const auto& estate : filtered) {
        REQUIRE(estate.isComplete());
        REQUIRE(estate.getPrice() >= 200000);
        REQUIRE(estate.getPrice() <= 600000);
    }
}