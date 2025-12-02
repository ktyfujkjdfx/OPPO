#ifndef REALESTATESORTER_H
#define REALESTATESORTER_H

#include "RealEstate.h"
#include <vector>

class RealEstateSorter {
public:
    static void sortByDateAscending(std::vector<RealEstate>& properties);
    static void sortByDateDescending(std::vector<RealEstate>& properties);
};

#endif 