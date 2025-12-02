#include "RealEstate.h"
#include <iostream>

using std::string;
using std::cout;
using std::endl;

void RealEstate::setProperty(const string& prop) { property = prop; }
void RealEstate::setOwner(const string& own) { owner = own; }
void RealEstate::setDate(const string& dt) { date = dt; }
void RealEstate::setPrice(const string& pr) {
    price = pr;
    try {
        price_int = std::stoi(pr);
    }
    catch (const std::exception&) {
        price_int = 0; // Устанавливаем 0 при ошибке
    }
}

const string& RealEstate::getProperty() const { return property; }
const string& RealEstate::getOwner() const { return owner; }
const string& RealEstate::getDate() const { return date; }
const string& RealEstate::getPrice() const { return price; }
int RealEstate::getPriceInt() const { return price_int; }

void RealEstate::print() const {
    cout << "Недвижимость: " << property
        << "\nВладелец: " << owner
        << "\nДата: " << date
        << "\nСтоимость: " << price << " руб.\n"
        << "\n";
}

bool RealEstate::isComplete() const {
    return !property.empty() && !owner.empty() && !date.empty() && !price.empty();
}

void RealEstate::clear() {
    property.clear();
    owner.clear();
    date.clear();
    price.clear();
    price_int = 0;
}