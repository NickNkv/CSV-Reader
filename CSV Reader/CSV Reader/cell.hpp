#pragma once
#include "types.hpp"

class Cell {
public:
	//lifecycle
	Cell();
	Cell(const char* value);
	Cell(const Cell& other);
	~Cell();

	Cell& operator = (const Cell& other);

	//getters & setters
	const char* getValue() const { return value; }
	void setValue(const char* value);

	//mechanics

private:
	char* value;
};