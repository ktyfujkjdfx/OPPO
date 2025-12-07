#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>

#include "RealEstate.h"
#include "RealEstateParser.h"
#include "RealEstateFilter.h"
#include "RealEstateSorter.h"

struct MenuItem {
    std::string description;
    std::function<void()> action;
};

// Объявление функций
void FilterByPriceMenu(std::vector<RealEstate>& properties);
void SortByDateMenu(std::vector<RealEstate>& properties);
void ShowAllProperties(std::vector<RealEstate>& properties);

int main() {
    std::setlocale(LC_ALL, "ru");

    std::vector<RealEstate> properties;

    try {
        RealEstateParser parser;
        properties = parser.ParseFile("text.txt");
    }
    catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    RealEstateSorter::SortByDateDescending(properties);

    std::cout << "СПИСОК ОБЪЕКТОВ НЕДВИЖИМОСТИ(" << properties.size() << " шт)"
        << std::endl;
    for (size_t i = 0; i < properties.size(); ++i) {
        std::cout << "[" << (i + 1) << "] " << properties[i].GetProperty()
            << " | Цена: " << properties[i].GetPrice() << " руб."
            << " | Дата: " << properties[i].GetDate() << std::endl;
    }
    std::cout << std::endl;

    int menu_choice;

    std::map<int, MenuItem> menu = {
        {1, {"Фильтр по цене", [&]() { FilterByPriceMenu(properties); }}},
        {2, {"Сортировка по дате", [&]() { SortByDateMenu(properties); }}},
        {3, {"Показать все объекты",
             [&]() { ShowAllProperties(properties); }}} };

    std::cout << "\n=== ВЫБЕРИТЕ ДЕЙСТВИЕ ===" << std::endl;
    for (const auto& item : menu) {
        std::cout << item.first << "-" << item.second.description << std::endl;
    }
    std::cout << "Выберите действие (1-" << menu.size() << "): ";
    std::cin >> menu_choice;

    auto it = menu.find(menu_choice);
    if (it != menu.end()) {
        it->second.action();
    }
    else {
        std::cout << "Неверный выбор! Показываю все объекты.\n" << std::endl;
        for (size_t i = 0; i < properties.size(); ++i) {
            std::cout << "ОБЪЕКТ #" << (i + 1) << "\n";
            properties[i].Print();
        }
    }

    return 0;
}

// Реализации функций

void FilterByPriceMenu(std::vector<RealEstate>& properties) {
    int min_price;
    int max_price;

    std::cout << "Введите минимальную цену: ";
    std::cin >> min_price;
    std::cout << "Введите максимальную цену: ";
    std::cin >> max_price;

    if (min_price > max_price) {
        std::cout << "Корректирую значения..." << std::endl;
        std::swap(min_price, max_price);
    }

    std::vector<RealEstate> filtered_properties =
        RealEstateFilter::FilterByPriceRange(properties, min_price, max_price);

    if (filtered_properties.empty()) {
        std::cout << "\nВ указаном диапазоне объектов не найдено" << std::endl;
    }
    else {
        std::cout << "\nНАЙДЕНО ОБЪЕКТОВ: " << filtered_properties.size()
            << std::endl;
        for (size_t i = 0; i < filtered_properties.size(); ++i) {
            std::cout << i + 1 << " " << filtered_properties[i].GetProperty()
                << " | " << filtered_properties[i].GetPrice() << " руб."
                << " | " << filtered_properties[i].GetDate() << std::endl;
        }
    }
}

void SortByDateMenu(std::vector<RealEstate>& properties) {
    char sort_order;
    std::cout << "\nПорядок сортировки:\n1 - От старых к новым\n"
        << "2 - От новых к старым\nВыберите: ";
    std::cin >> sort_order;

    std::vector<RealEstate> sorted_properties = properties;

    if (sort_order == '1') {
        RealEstateSorter::SortByDateAscending(sorted_properties);
        std::cout << "\nОБЪЕКТЫ ОТСОРТИРОВАНЫ ПО ВОЗРАСТАНИЮ ДАТЫ:\n"
            << std::endl;
    }
    else if (sort_order == '2') {
        RealEstateSorter::SortByDateDescending(sorted_properties);
        std::cout << "\nОБЪЕКТЫ ОТСОРТИРОВАНЫ ПО УБЫВАНИЮ ДАТЫ:\n"
            << std::endl;
    }
    else {
        std::cout << "Неверный выбор! Показываю без сортировки\n" << std::endl;
        sorted_properties = properties;
    }

    for (size_t i = 0; i < sorted_properties.size(); ++i) {
        std::cout << "ОБЪЕКТ #" << (i + 1) << " (дата: "
            << sorted_properties[i].GetDate() << ", цена: "
            << sorted_properties[i].GetPrice() << " руб.): "
            << sorted_properties[i].GetProperty() << std::endl;
    }
}

void ShowAllProperties(std::vector<RealEstate>& properties) {
    std::cout << "\nВСЕ ОБЪЕКТЫ (отсортированы по цене):\n" << std::endl;
    for (size_t i = 0; i < properties.size(); ++i) {
        std::cout << "[" << (i + 1) << "] " << properties[i].GetProperty()
            << " | Дата: " << properties[i].GetDate() << " | Цена: "
            << properties[i].GetPrice() << " руб." << std::endl;
    }
}