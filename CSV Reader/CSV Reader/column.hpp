#pragma once
#include "cell.hpp"
#include "types.hpp"

//The task of having unique column names falls onto class Table!

class Column {
public:
	//lifecycle
	Column() = delete;
	Column(size_t size, ColumnType type, const char* name);
	Column(const Column& other);
	~Column();

	Column& operator = (const Column& other);

	//getters & setters
	size_t getSize() const { return size; }
	//size setter - not allowed!

	ColumnType getType() const { return type; }
	void setType();
	 
	const char* getName() const { return name; }
	void setName(const char* name);

	//mechanics
	void insertCellAt(size_t index, Cell& cell);
	void addCell(Cell& cell);
	const Cell** getCells();
	const Cell* getCellAt(size_t index);


private:
	size_t size; //number of elements
	size_t allocatedCapacity;
	ColumnType type;
	char* name; //column header name
	Cell** cells;
};