#ifndef REALESTATEPARSER_H
#define REALESTATEPARSER_H

#include <regex>
#include <string>
#include <vector>

#include "RealEstate.h"

class RealEstateParser {
public:
	RealEstateParser();
	std::vector<RealEstate> ParseFile(const std::string& filename);
	std::vector<RealEstate> ParseFromString(const std::string& data);

private:
	const std::regex OBJECT_PATTERN;
	const std::regex OWNER_PATTERN;
	const std::regex DATE_PATTERN;
	const std::regex PRICE_PATTERN;
};

#endif