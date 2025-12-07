#ifndef REALESTATESORTER_H
#define REALESTATESORTER_H

#include <vector>

#include "RealEstate.h"

class RealEstateSorter {
public:
	static void SortByDateAscending(std::vector<RealEstate>& properties);
	static void SortByDateDescending(std::vector<RealEstate>& properties);
};

#endif  