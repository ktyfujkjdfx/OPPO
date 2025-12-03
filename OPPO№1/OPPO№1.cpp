#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include "RealEstate.h"
#include "RealEstateParser.h"
#include "RealEstateFilter.h"
#include "RealEstateSorter.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::cin;

struct MenuItem {
    string description;
    std::function<void()> action;
};

// объявление функций
void filterByPriceMenu(vector<RealEstate>& properties);
void sortByDateMenu(vector<RealEstate>& properties);
void showAllProperties(vector<RealEstate>& properties);

int main_pr() {
    std::setlocale(LC_ALL, "ru");

    vector<RealEstate> properties;

    try {
        RealEstateParser parser;
        properties = parser.parseFile("text.txt");
    }
    catch (const std::exception& e) {
        cout << "Ошибка: " << e.what() << endl;
        return 1;
    }

    RealEstateSorter::sortByDateDescending(properties);

    cout << "СПИСОК ОБЪЕКТОВ НЕДВИЖИМОСТИ (" << properties.size() << " шт.)" << endl;
    for (size_t i = 0; i < properties.size(); i++) {
        cout << "[" << (i + 1) << "] " << properties[i].getProperty()
            << " | Цена: " << properties[i].getPrice() << " руб."
            << " | Дата: " << properties[i].getDate() << endl;
    }
    cout << endl;

    int menu_choice;

    std::map<int, MenuItem> menu = {
        {1, {"Фильтр по цене", [&]() { filterByPriceMenu(properties); }}},
        {2, {"Сортировка по дате", [&]() { sortByDateMenu(properties); }}},
        {3, {"Показать все объекты", [&]() { showAllProperties(properties); }}}
    };

    cout << "\n=== ВЫБЕРИТЕ ДЕЙСТВИЕ ===" << endl;
    for (const auto& item : menu) {
        cout << item.first << " - " << item.second.description << endl;
    }
    cout << "Выберите действие (1-" << menu.size() << "): ";
    cin >> menu_choice;

    auto it = menu.find(menu_choice);
    if (it != menu.end()) {
        it->second.action();
    }
    else {
        cout << "Неверный выбор! Показываю все объекты.\n" << endl;
        for (size_t i = 0; i < properties.size(); i++) {
            cout << "ОБЪЕКТ #" << (i + 1) << "\n";
            properties[i].print();
        }
    }

    return 0;
}

// Реализации функций

void filterByPriceMenu(vector<RealEstate>& properties) {
    int min_price, max_price;

    cout << "Введите минимальную цену: ";
    cin >> min_price;
    cout << "Введите максимальную цену: ";
    cin >> max_price;

    if (min_price > max_price) {
        cout << "Корректирую значения..." << endl;
        std::swap(min_price, max_price);
    }

    vector<RealEstate> filtered_properties =
        RealEstateFilter::filterByPriceRange(properties, min_price, max_price);

    if (filtered_properties.empty()) {
        cout << "\nВ указанном диапазоне объектов не найдено." << endl;
    }
    else {
        cout << "\nНАЙДЕНО ОБЪЕКТОВ: " << filtered_properties.size() << endl;
        for (size_t i = 0; i < filtered_properties.size(); i++) {
            cout << i + 1 << " " << filtered_properties[i].getProperty()
                << " | " << filtered_properties[i].getPrice() << " руб."
                << " | " << filtered_properties[i].getDate() << endl;
        }
    }
}

void sortByDateMenu(vector<RealEstate>& properties) {
    char sort_order;
    cout << "\nПорядок сортировки:\n1 - От старых к новым\n2 - От новых к старым\nВыберите: ";
    cin >> sort_order;

    vector<RealEstate> sorted_properties = properties;

    if (sort_order == '1') {
        RealEstateSorter::sortByDateAscending(sorted_properties);
        cout << "\nОБЪЕКТЫ ОТСОРТИРОВАНЫ ПО ВОЗРАСТАНИЮ ДАТЫ:\n" << endl;
    }
    else if (sort_order == '2') {
        RealEstateSorter::sortByDateDescending(sorted_properties);
        cout << "\nОБЪЕКТЫ ОТСОРТИРОВАНЫ ПО УБЫВАНИЮ ДАТЫ:\n" << endl;
    }
    else {
        cout << "Неверный выбор! Показываю без сортировки.\n" << endl;
        sorted_properties = properties;
    }
    for (size_t i = 0; i < sorted_properties.size(); i++) {
        cout << "ОБЪЕКТ #" << (i + 1)
            << " (дата: " << sorted_properties[i].getDate()
            << ", цена: " << sorted_properties[i].getPrice() << " руб.): "
            << sorted_properties[i].getProperty() << endl;
    }
}

void showAllProperties(vector<RealEstate>& properties) {
    cout << "\nВСЕ ОБЪЕКТЫ (отсортированы по цене):\n" << endl;
    for (size_t i = 0; i < properties.size(); i++) {
        cout << "[" << (i + 1) << "] "
            << properties[i].getProperty()
            << " | Дата: " << properties[i].getDate()
            << " | Цена: " << properties[i].getPrice() << " руб." << endl;
    }
}