#ifndef REALESTATEFILTER_H
#define REALESTATEFILTER_H

#include "RealEstate.h"
#include <vector>

class RealEstateFilter {
public:
    static std::vector<RealEstate> filterByPriceRange(const std::vector<RealEstate>& properties,
        int min_price, int max_price);
};

#endif