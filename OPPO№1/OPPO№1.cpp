#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <map>
#include <functional>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::cin;

// ВЫНЕСЕНО: глобальные константы регулярных выражений
const std::regex OBJECT_PATTERN(R"((Object|OBJECT)\s*#?\s*\d+:\s*([^\.\n]+))", std::regex_constants::icase);
const std::regex OWNER_PATTERN(R"(\"([^\"]+)\")");
const std::regex DATE_PATTERN(R"((\d{4}\.\d{2}\.\d{2}))");
const std::regex PRICE_PATTERN(R"((?:price|cost)[^\d]*?(\d+))", std::regex_constants::icase);

class RealEstate {
private:
    string property;
    string owner;
    string date;
    string price;
    int price_int = 0;

public:
    void setProperty(const string& prop) { property = prop; }
    void setOwner(const string& own) { owner = own; }
    void setDate(const string& dt) { date = dt; }
    void setPrice(const string& pr) {
        price = pr;
        price_int = std::stoi(pr);
    }

    const string& getProperty() const { return property; }
    const string& getOwner() const { return owner; }
    const string& getDate() const { return date; }
    const string& getPrice() const { return price; }
    int getPriceInt() const { return price_int; }

    void print() const {
        cout << "Недвижимость: " << property
            << "\nВладелец: " << owner
            << "\nДата: " << date
            << "\nСтоимость: " << price << " руб.\n"
            << "\n";
    }

    bool isComplete() const {
        return !property.empty() && !owner.empty() && !date.empty() && !price.empty();
    }

    void clear() {
        property.clear();
        owner.clear();
        date.clear();
        price.clear();
        price_int = 0;
    }
};

vector<RealEstate> filterByPriceRange(const vector<RealEstate>& properties, int min_price, int max_price) {
    vector<RealEstate> filtered;
    for (const auto& prop : properties) {
        int price = prop.getPriceInt();
        if (price >= min_price && price <= max_price) {
            filtered.push_back(prop);
        }
    }
    return filtered;
}

void sortByDateAscending(vector<RealEstate>& properties) {
    std::sort(properties.begin(), properties.end(),
        [](const RealEstate& a, const RealEstate& b) {
            return a.getDate() < b.getDate();
        });
}

void sortByDateDescending(vector<RealEstate>& properties) {
    std::sort(properties.begin(), properties.end(),
        [](const RealEstate& a, const RealEstate& b) {
            return a.getDate() > b.getDate();
        });
}

struct MenuItem {
    string description;
    std::function<void()> action;
};

int main() {
    std::setlocale(LC_ALL, "ru");

    std::ifstream file("text.txt");
    if (!file.is_open()) {
        cout << "Файл не открылся" << endl;
        return 1;
    }

    vector<RealEstate> properties;
    RealEstate current;
    string line;

    // УДАЛЕНО: объявление regex внутри main()
    // ИСПОЛЬЗУЕМ глобальные константы

    while (std::getline(file, line)) {
        std::smatch matches;

        if (std::regex_search(line, matches, OBJECT_PATTERN)) {
            if (current.isComplete()) {
                properties.push_back(current);
                current.clear();
            }
            current.setProperty(matches[2]);
        }

        if (std::regex_search(line, matches, OWNER_PATTERN) && current.getOwner().empty()) {
            current.setOwner(matches[1]);
        }

        if (std::regex_search(line, matches, DATE_PATTERN) && current.getDate().empty()) {
            current.setDate(matches[1]);
        }

        if (std::regex_search(line, matches, PRICE_PATTERN) && current.getPrice().empty()) {
            current.setPrice(matches[1]);
        }
    }

    if (current.isComplete()) {
        properties.push_back(current);
    }

    file.close();

    // Сортирую по цене по убыванию (исходная сортировка)
    std::sort(properties.begin(), properties.end(), [](const RealEstate& a, const RealEstate& b) {
        return a.getPriceInt() > b.getPriceInt();
        });


    // ИЗМЕНЕНО: формат вывода объектов
    cout << "СПИСОК ОБЪЕКТОВ НЕДВИЖИМОСТИ (" << properties.size() << " шт.)" << endl;
    for (size_t i = 0; i < properties.size(); i++) {
        cout << "[" << (i + 1) << "] " << properties[i].getProperty()
            << " | Цена: " << properties[i].getPrice() << " руб."
            << " | Дата: " << properties[i].getDate() << endl;
    }
    cout << endl;

    int min_price, max_price;
    int menu_choice;

    // ДОБАВЛЕНО: map-меню вместо switch-case
    std::map<int, MenuItem> menu = {
        {1, {"Фильтр по цене", [&]() {
            cout << "Введите минимальную цену: ";
            cin >> min_price;
            cout << "Введите максимальную цену: ";
            cin >> max_price;

            if (min_price > max_price) {
                cout << "Корректирую значения..." << endl;
                std::swap(min_price, max_price);
            }

            vector<RealEstate> filtered_properties = filterByPriceRange(properties, min_price, max_price);

            if (filtered_properties.empty()) {
                cout << "\nВ указанном диапазоне объектов не найдено." << endl;
            }
            else {
                // ИЗМЕНЕНО: компактный формат вывода
                cout << "\nНАЙДЕНО ОБЪЕКТОВ: " << filtered_properties.size() << endl;
                for (size_t i = 0; i < filtered_properties.size(); i++) {
                    cout << i + 1 << " " << filtered_properties[i].getProperty()
                        << " | " << filtered_properties[i].getPrice() << " руб."
                        << " | " << filtered_properties[i].getDate() << endl;
                }
            }
        }}},

        {2, {"Сортировка по дате", [&]() {
            char sort_order;
            cout << "\nПорядок сортировки:\n1 - От старых к новым\n2 - От новых к старым\nВыберите: ";
            cin >> sort_order;

            if (sort_order == '1') {
                sortByDateAscending(properties);
                cout << "\nОБЪЕКТЫ ОТСОРТИРОВАНЫ ПО ВОЗРАСТАНИЮ ДАТЫ:\n" << endl;
            }
            else if (sort_order == '2') {
                sortByDateDescending(properties);
                cout << "\nОБЪЕКТЫ ОТСОРТИРОВАНЫ ПО УБЫВАНИЮ ДАТЫ:\n" << endl;
            }
            else {
                cout << "Неверный выбор! Показываю без сортировки.\n" << endl;
            }

            // Показываю отсортированный список
            for (size_t i = 0; i < properties.size(); i++) {
                cout << "ОБЪЕКТ #" << (i + 1)
                     << " (дата: " << properties[i].getDate()
                     << ", цена: " << properties[i].getPrice() << " руб.): "
                     << properties[i].getProperty() << endl;
            }
        }}},

        {3, {"Показать все объекты", [&]() {
            cout << "\nВСЕ ОБЪЕКТЫ (отсортированы по цене):\n" << endl;
            for (size_t i = 0; i < properties.size(); i++) {
                cout << "[" << (i + 1) << "] "
                     << properties[i].getProperty()
                     << " | Дата: " << properties[i].getDate()
                     << " | Цена: " << properties[i].getPrice() << " руб." << endl;
            }
        }}}
    };

    // ИЗМЕНЕНО: вывод меню через map
    cout << "\n=== ВЫБЕРИТЕ ДЕЙСТВИЕ ===" << endl;
    for (const auto& item : menu) {
        cout << item.first << " - " << item.second.description << endl;
    }
    cout << "Выберите действие (1-" << menu.size() << "): ";
    cin >> menu_choice;

    // ИЗМЕНЕНО: обработка выбора через map вместо switch
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