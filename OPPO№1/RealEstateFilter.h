#ifndef REALESTATEFILTER_H
#define REALESTATEFILTER_H

#include <vector>

#include "RealEstate.h"

class RealEstateFilter {
public:
    static std::vector<RealEstate> FilterByPriceRange(
        const std::vector<RealEstate>& properties,
        int min_price,
        int max_price);
};

#endif  