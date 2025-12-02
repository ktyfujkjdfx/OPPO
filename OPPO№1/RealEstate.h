#ifndef REALESTATE_H
#define REALESTATE_H

#include <string>

class RealEstate {
private:
    std::string property;
    std::string owner;
    std::string date;
    std::string price;
    int price_int = 0;

public:
    void setProperty(const std::string& prop);
    void setOwner(const std::string& own);
    void setDate(const std::string& dt);
    void setPrice(const std::string& pr);

    const std::string& getProperty() const;
    const std::string& getOwner() const;
    const std::string& getDate() const;
    const std::string& getPrice() const;
    int getPriceInt() const;

    void print() const;
    bool isComplete() const;
    void clear();
};

#endif 