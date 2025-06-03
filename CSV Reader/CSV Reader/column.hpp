#pragma once
#include "cell.hpp"

class Column {
public:
	//lifecycle
	Column();

private:
	Cell* cells;
	size_t size; 
	size_t allocatedCapacity;
};