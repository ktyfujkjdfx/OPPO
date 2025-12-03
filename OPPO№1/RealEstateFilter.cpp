#include "RealEstateFilter.h"

std::vector<RealEstate> RealEstateFilter::filterByPriceRange(
    const std::vector<RealEstate>& properties, 
    int min_price, 
    int max_price) {
    
    std::vector<RealEstate> filtered;
    for (const auto& prop : properties) {
        int price = prop.getPrice();
        if (price >= min_price && price <= max_price) {
            filtered.push_back(prop);
        }
    }
    return filtered;
}