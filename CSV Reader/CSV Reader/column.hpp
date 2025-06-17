#pragma once
#include "cell.hpp"
#include "types.hpp"

//The task of having unique column names falls onto class Table!

class Column {
public:
	//lifecycle
	Column();
	Column(ColumnType type, const char* name);
	Column(const Column& other);
	~Column();

	Column& operator = (const Column& other);
	Cell* operator[] (size_t index);
	const Cell* operator[] (size_t index) const;

	//getters & setters
	size_t getSize() const { return size; }
	//size setter - not allowed!

	ColumnType getType() const { return type; }
	void setType();
	 
	const char* getName() const { return name; }
	void setName(const char* name);

	unsigned int getWidth() const { return width; }
	//width setter - not allowed!

	//mechanics
	bool insertCellAt(size_t index, Cell& cell);
	bool addCell(Cell& cell);
	const Cell* getCellAt(size_t index);
	void removeCellAt(size_t index);

private:
	void expandCollection(); //allocates new [size + BONUS_CAPACITY] cell* 

private:
	size_t size; //number of elements
	size_t allocatedCapacity;
	ColumnType type;
	char* name; //column header name
	Cell** cells;
	unsigned int width; //keeps the length of the longest value (out of all cells in this col), used to later display the table (spacings)
};