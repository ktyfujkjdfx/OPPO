#ifndef REALESTATEPARSER_H
#define REALESTATEPARSER_H

#include "RealEstate.h"
#include <vector>
#include <string>
#include <regex>

class RealEstateParser {
private:
    const std::regex OBJECT_PATTERN;
    const std::regex OWNER_PATTERN;
    const std::regex DATE_PATTERN;
    const std::regex PRICE_PATTERN;

public:
    RealEstateParser();
    std::vector<RealEstate> parseFile(const std::string& filename);
    std::vector<RealEstate> parseFromString(const std::string& data);
};

#endif