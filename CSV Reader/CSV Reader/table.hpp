#pragma once
#include "column.hpp"

class Table {
public:
	Table();
	Table(const char* name);
	Table(const Table& other);
	~Table();

	Table& operator= (const Table& other);

	//getters & setters
	const char* getName() const { return name; }
	void setName();

	size_t getColCount() const { return colCount; }
	//colCoint setter is NOT allowed

	size_t getRowCount() const { return rowCount; }
	//rowCount setter is NOT allowed

	const char* getDelimiter() const { return delimiter; }
	void setDelimiter(const char* delimiter);

	//mechanics

private:
	char* name;
	size_t colCount;
	size_t rowCount;
	size_t allocatedCapacity;
	char* delimiter;
	Column** columns;
};