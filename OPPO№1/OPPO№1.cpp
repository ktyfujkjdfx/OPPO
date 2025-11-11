 #include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::cin;

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

// Функции сортировки по дате
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

    std::regex object_pattern(R"((Object|OBJECT)\s*#?\s*\d+:\s*([^\.\n]+))", std::regex_constants::icase);
    std::regex owner_pattern(R"(\"([^\"]+)\")");
    std::regex date_pattern(R"((\d{4}\.\d{2}\.\d{2}))");
    std::regex price_pattern(R"((?:price|cost)[^\d]*?(\d+))", std::regex_constants::icase);

    while (std::getline(file, line)) {
        std::smatch matches;

        if (std::regex_search(line, matches, object_pattern)) {
            if (current.isComplete()) {
                properties.push_back(current);
                current.clear();
            }
            current.setProperty(matches[2]);
        }

        if (std::regex_search(line, matches, owner_pattern) && current.getOwner().empty()) {
            current.setOwner(matches[1]);
        }

        if (std::regex_search(line, matches, date_pattern) && current.getDate().empty()) {
            current.setDate(matches[1]);
        }

        if (std::regex_search(line, matches, price_pattern) && current.getPrice().empty()) {
            current.setPrice(matches[1]);
        }
    }

    if (current.isComplete()) {
        properties.push_back(current);
    }

    file.close();

    // Сортируем по цене по убыванию (исходная сортировка)
    std::sort(properties.begin(), properties.end(), [](const RealEstate& a, const RealEstate& b) {
        return a.getPriceInt() > b.getPriceInt();
        });


    cout << "ВСЕ ОБЪЕКТЫ НЕДВИЖИМОСТИ (" << properties.size() << " шт.)" << endl;
    for (size_t i = 0; i < properties.size(); i++) {
        cout << "ОБЪЕКТ #" << (i + 1) << " (цена: " << properties[i].getPrice() << " руб., дата: " << properties[i].getDate() << "): "
            << properties[i].getProperty() << endl;
    }
    cout << endl;

    int min_price, max_price;
    char menu_choice;

    cout << "\n=== ВЫБЕРИТЕ ДЕЙСТВИЕ ===" << endl;
    cout << "1 - Фильтр по цене" << endl;
    cout << "2 - Сортировка по дате" << endl;
    cout << "3 - Показать все объекты" << endl;
    cout << "Выберите действие (1-3): ";
    cin >> menu_choice;

    switch (menu_choice) {
        case '1': {
            // Фильтр по цене
            cout << "Введите минимальную цену: ";
            cin >> min_price;
            cout << "Введите максимальную цену: ";
            cin >> max_price;

            if (min_price > max_price) {
                cout << "Ошибка: минимальная цена не может быть больше максимальной!" << endl;
                cout << "Меняю местами..." << endl;
                std::swap(min_price, max_price);
            }

            vector<RealEstate> filtered_properties = filterByPriceRange(properties, min_price, max_price);

            if (filtered_properties.empty()) {
                cout << "\nОбъектов в диапазоне от " << min_price << " до " << max_price << " руб. не найдено." << endl;
            }
            else {
                cout << "\nОБЪЕКТЫ В ДИАПАЗОНЕ ОТ " << min_price << " ДО " << max_price << " РУБ: \n" << endl;
                for (size_t i = 0; i < filtered_properties.size(); i++) {
                    cout << "Объект #" << (i + 1) << ":\n";
                    filtered_properties[i].print();
                }
            }
            break;
        }
        
        case '2': {
            // Сортировка по дате
            char sort_order;
            cout << "\n=== СОРТИРОВКА ПО ДАТЕ ===" << endl;
            cout << "1 - По возрастанию (от старых к новым)" << endl;
            cout << "2 - По убыванию (от новых к старым)" << endl;
            cout << "Выберите порядок сортировки (1-2): ";
            cin >> sort_order;
            
            if (sort_order == '1') {
                sortByDateAscending(properties);
                cout << "\nОБЪЕКТЫ ОТСОРТИРОВАНЫ ПО ВОЗРАСТАНИЮ ДАТЫ:\n" << endl;
            } else if (sort_order == '2') {
                sortByDateDescending(properties);
                cout << "\nОБЪЕКТЫ ОТСОРТИРОВАНЫ ПО УБЫВАНИЮ ДАТЫ:\n" << endl;
            } else {
                cout << "Неверный выбор! Показываю без сортировки.\n" << endl;
            }
            
            // Показываем отсортированный список
            for (size_t i = 0; i < properties.size(); i++) {
                cout << "ОБЪЕКТ #" << (i + 1) 
                     << " (дата: " << properties[i].getDate() 
                     << ", цена: " << properties[i].getPrice() << " руб.): "
                     << properties[i].getProperty() << endl;
            }
            break;
        }
        
        case '3': {
            // Показываем все объекты (исходный порядок по цене)
            cout << "\nВСЕ ОБЪЕКТЫ (отсортированы по цене):\n" << endl;
            for (size_t i = 0; i < properties.size(); i++) {
                cout << "ОБЪЕКТ #" << (i + 1) << "\n";
                properties[i].print();
            }
            break;
        }
        
        default: {
            cout << "Неверный выбор! Показываю все объекты.\n" << endl;
            for (size_t i = 0; i < properties.size(); i++) {
                cout << "ОБЪЕКТ #" << (i + 1) << "\n";
                properties[i].print();
            }
            break;
        }
    }

    return 0;
}