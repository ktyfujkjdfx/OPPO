#include "RealEstateFilter.h"

#include <vector>

#include "RealEstate.h"

std::vector<RealEstate> RealEstateFilter::FilterByPriceRange(
    const std::vector<RealEstate>& properties, int min_price, int max_price) {
    std::vector<RealEstate> filtered;
    for (const auto& prop : properties) {
        int price = prop.GetPrice();
        if (price >= min_price && price <= max_price) {
            filtered.push_back(prop);
        }
    }
    return filtered;
}