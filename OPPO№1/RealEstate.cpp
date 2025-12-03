#include "RealEstate.h"
#include "date_utils.h"  
#include <iostream>

using std::string;
using std::cout;
using std::endl;

void RealEstate::setProperty(const string& prop) { property = prop; }
void RealEstate::setOwner(const string& own) { owner = own; }

// добавил проверку даты
void RealEstate::setDate(const string& dt) {
    if (isValidDate(dt)) {
        date = dt;
    }
    else {
        date.clear();  // Если дата некорректная - очищаю
    }
}

void RealEstate::setPrice(const string& pr) {
    try {
        long long price_val = std::stoll(pr);  // Используем long long
        if (price_val < 0) {
            price_int = 0;
            price_set = false;
        }
        else if (price_val > 1000000000) {  // Лимит 1 млрд
            price_int = 1000000000;
            price_set = true;
        }
        else {
            price_int = static_cast<int>(price_val);
            price_set = true;
        }
    }
    catch (const std::exception&) {
        price_int = 0;
        price_set = false;
    }
}

const string& RealEstate::getProperty() const { return property; }
const string& RealEstate::getOwner() const { return owner; }
const string& RealEstate::getDate() const { return date; }

int RealEstate::getPrice() const {
    return price_int;
}

void RealEstate::print() const {
    cout << "Недвижимость: " << property
        << "\nВладелец: " << owner
        << "\nДата: " << date
        << "\nСтоимость: " << price_int << " руб.\n"
        << "\n";
}

bool RealEstate::isComplete() const {
    return !property.empty() && !owner.empty() && !date.empty() && price_set;
}

void RealEstate::clear() {
    property.clear();
    owner.clear();
    date.clear();
    price_int = 0;
    price_set = false;  
}