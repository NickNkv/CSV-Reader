#pragma once
#include "column.hpp"

class Table {
public:

private:
	char* name;
	size_t colCount;
	size_t rowCount;
	char* delimeter;
	Column** columns;
};