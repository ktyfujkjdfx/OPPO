#ifndef REALESTATE_H
#define REALESTATE_H

#include <string>

class RealEstate {
private:
    std::string property;
    std::string owner;
    std::string date;
    int price_int = 0;
    bool price_set = false;

public:
    void setProperty(const std::string& prop);
    void setOwner(const std::string& own);
    void setDate(const std::string& dt);
    void setPrice(const std::string& pr);

    const std::string& getProperty() const;
    const std::string& getOwner() const;
    const std::string& getDate() const;
    int getPrice() const;

    void print() const;
    bool isComplete() const;
    void clear();
};

#endif