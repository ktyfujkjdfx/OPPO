#include "RealEstateParser.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include "date_utils.h"

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

RealEstateParser::RealEstateParser()
    : OBJECT_PATTERN(
        R"((Object|OBJECT)\s*#?\s*\d+\s*:\s*([^\.\n]+)\.?)",
        std::regex_constants::icase),
    OWNER_PATTERN(R"(\"([^\"]+)\")"),
    DATE_PATTERN(R"((\d{4}\.\d{2}\.\d{2}))"),
    PRICE_PATTERN(
        R"((?:price|cost|Price|Cost|стоимость)\s*:?\s*(\d+))") {}

std::vector<RealEstate> RealEstateParser::ParseFile(
    const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Файл не открылся: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return ParseFromString(buffer.str());
}

std::vector<RealEstate> RealEstateParser::ParseFromString(
    const std::string& data) {
    std::vector<RealEstate> properties;

    std::istringstream stream(data);
    std::string line;
    RealEstate current;
    bool in_object = false;

    while (std::getline(stream, line)) {
        std::smatch matches;

        if (std::regex_search(line, matches, OBJECT_PATTERN) &&
            matches.size() > 2) {
            if (current.IsComplete()) {
                properties.push_back(current);
            }
            current.Clear();
            in_object = true;
            current.SetProperty(trim(matches[2].str()));
        }

        if (!in_object) continue;

        if (current.GetOwner().empty() &&
            std::regex_search(line, matches, OWNER_PATTERN) &&
            matches.size() > 1) {
            current.SetOwner(trim(matches[1].str()));
        }

        // добавил проверку даты
        if (current.GetDate().empty() &&
            std::regex_search(line, matches, DATE_PATTERN) &&
            matches.size() > 1) {
            std::string found_date = trim(matches[1].str());
            if (IsValidDate(found_date)) {
                current.SetDate(found_date);
            }
        }

        if (current.GetPrice() == 0) {
            if (std::regex_search(line, matches, PRICE_PATTERN) &&
                matches.size() > 1) {
                current.SetPrice(trim(matches[1].str()));
            }
            else if (!current.GetOwner().empty() &&
                !current.GetDate().empty()) {
                std::regex number_pattern(R"(\b(\d{5,})\b)");
                std::sregex_iterator it(line.begin(), line.end(), number_pattern);
                std::sregex_iterator end;

                for (; it != end; ++it) {
                    std::string price_str = (*it)[1].str();

                    if (line.find(price_str) != std::string::npos) {
                        size_t pos = line.find(price_str);
                        if ((pos > 0 && line[pos - 1] == '.') ||
                            (pos + price_str.length() < line.length() &&
                                line[pos + price_str.length()] == '.')) {
                            continue;
                        }
                    }

                    int price_val = std::stoi(price_str);
                    if (price_val >= 10000 && price_val <= 100000000) {
                        current.SetPrice(price_str);
                        break;
                    }
                }
            }
        }

        if (current.IsComplete()) {
            properties.push_back(current);
            current.Clear();
            in_object = false;
        }
    }

    if (current.IsComplete()) {
        properties.push_back(current);
    }

    return properties;
}