#pragma once
#include "types.hpp"

class Cell {
public:
	//lifecycle
	Cell();
	Cell(const char* value, ColumnType dataType);
	Cell(const Cell& other);
	~Cell();

	//getters & setters

private:
	char* value;
	ColumnType dataType;
};