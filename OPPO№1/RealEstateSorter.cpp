#include "RealEstateSorter.h"

#include <algorithm>

void RealEstateSorter::SortByDateAscending(
    std::vector<RealEstate>& properties) {
    std::sort(properties.begin(), properties.end(),
        [](const RealEstate& a, const RealEstate& b) {
            return a.GetDate() < b.GetDate();
        });
}

void RealEstateSorter::SortByDateDescending(
    std::vector<RealEstate>& properties) {
    std::sort(properties.begin(), properties.end(),
        [](const RealEstate& a, const RealEstate& b) {
            return a.GetDate() > b.GetDate();
        });
}