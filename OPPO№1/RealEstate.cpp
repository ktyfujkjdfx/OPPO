#include "RealEstate.h"

#include <iostream>
#include <string>

#include "date_utils.h"

void RealEstate::SetProperty(const std::string& prop) { property_ = prop; }

void RealEstate::SetOwner(const std::string& own) { owner_ = own; }

void RealEstate::SetDate(const std::string& dt) {
    if (IsValidDate(dt)) {
        date_ = dt;
    }
    else {
        date_.clear();  // Если дата некорректная - очищаю
    }
}

void RealEstate::SetPrice(const std::string& pr) {
    try {
        long long price_val = std::stoll(pr);  // Используем long long
        if (price_val < 0) {
            price_int_ = 0;
            price_set_ = false;
        }
        else if (price_val > 1000000000) {  // Лимит 1 млрд
            price_int_ = 1000000000;
            price_set_ = true;
        }
        else {
            price_int_ = static_cast<int>(price_val);
            price_set_ = true;
        }
    }
    catch (const std::exception&) {
        price_int_ = 0;
        price_set_ = false;
    }
}

const std::string& RealEstate::GetProperty() const { return property_; }

const std::string& RealEstate::GetOwner() const { return owner_; }

const std::string& RealEstate::GetDate() const { return date_; }

int RealEstate::GetPrice() const { return price_int_; }

void RealEstate::Print() const {
    std::cout << "Недвижимость: " << property_ << "\nВладелец: " << owner_
        << "\nДата: " << date_ << "\nСтоимость: " << price_int_
        << " руб.\n\n";
}

bool RealEstate::IsComplete() const {
    return !property_.empty() && !owner_.empty() && !date_.empty() &&
        price_set_;
}

void RealEstate::Clear() {
    property_.clear();
    owner_.clear();
    date_.clear();
    price_int_ = 0;
    price_set_ = false;
}