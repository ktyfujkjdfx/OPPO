#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <functional>

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
    RealEstate() = default;  // использую конструкторы

    RealEstate(const string& prop, const string& own, const string& dt, const string& pr)
        : property(prop), owner(own), date(dt) {
        setPrice(pr);
    }

    RealEstate(const string& prop, const string& own, const string& dt, int pr_int)
        : property(prop), owner(own), date(dt), price_int(pr_int) {
        price = std::to_string(pr_int);
    }

    // перегрузка операторов сравнения
    bool operator>(const RealEstate& other) const {
        return this->price_int > other.price_int;
    }

    bool operator<(const RealEstate& other) const {
        return this->price_int < other.price_int;
    }

    bool operator==(const RealEstate& other) const {
        return this->price_int == other.price_int;
    }

    // перегрузка оператора вывода
    friend std::ostream& operator<<(std::ostream& os, const RealEstate& estate) {
        os << "Недвижимость: " << estate.property
            << "\nВладелец: " << estate.owner
            << "\nДата: " << estate.date
            << "\nСтоимость: " << estate.price << " руб.\n";
        return os;
    }

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
        cout << *this;  // Используем перегруженный оператор <<
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

    // НОВАЯ СОРТИРОВКА с использованием перегруженного оператора >
    std::sort(properties.begin(), properties.end(), std::greater<RealEstate>());

    cout << "ВСЕ ОБЪЕКТЫ НЕДВИЖИМОСТИ (" << properties.size() << " шт.)" << endl;
    for (size_t i = 0; i < properties.size(); i++) {
        cout << "ОБЪЕКТ #" << (i + 1) << " (цена: " << properties[i].getPrice() << " руб.): "
            << properties[i].getProperty() << endl;
    }
    cout << endl;

    int min_price, max_price;
    char filter_choice;

    cout << "Хотите отфильтровать объекты по цене? (y/n): ";
    cin >> filter_choice;

    if (filter_choice == 'y' || filter_choice == 'Y') {
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
                // Используем перегруженный оператор << вместо print()
                cout << filtered_properties[i] << endl;
            }
        }
    }
    else {
        cout << "\n";
        for (size_t i = 0; i < properties.size(); i++) {
            cout << "ОБЪЕКТ #" << (i + 1) << "\n";
            // Используем перегруженный оператор << вместо print()
            cout << properties[i] << endl;
        }
    }

    return 0;
}