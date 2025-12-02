#include "RealEstateSorter.h"
#include <algorithm>

void RealEstateSorter::sortByDateAscending(std::vector<RealEstate>& properties) {
    std::sort(properties.begin(), properties.end(),
        [](const RealEstate& a, const RealEstate& b) {
            return a.getDate() < b.getDate();
        });
}

void RealEstateSorter::sortByDateDescending(std::vector<RealEstate>& properties) {
    std::sort(properties.begin(), properties.end(),
        [](const RealEstate& a, const RealEstate& b) {
            return a.getDate() > b.getDate();
        });
}