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
	void setName(const char* name);

	size_t getColCount() const { return colCount; }
	//colCoint setter is NOT allowed

	size_t getRowCount() const { return rowCount; }
	//rowCount setter is NOT allowed

	const char* getDelimiter() const { return delimiter; }
	void setDelimiter(const char* delimiter);

	bool isTableEmpty() const { return isEmpty; }

	const Cell* getCellAt(size_t row, size_t col);

	//mechanics
	bool addColumn(Column& column);
	void removeColumnAt(size_t index);
	//void copyColumn(size_t index);
	//void changeColumnOrder();
	//void removeDuplicateRows();
	//void changeColumnName(size_t index);
	//void editCell();

	bool populateTable(const char* fileName);
	bool saveToFile(const char* fileName);
	void printTable();
	bool sort(size_t index, bool ascending = true);
	long long findColByName(const char* name);
	bool swapRows(size_t first, size_t second); //indexes of the two rows
	void emptyTable();
	bool rowsEqual(size_t first, size_t second);
	void removeIdenticalRows();
	void removeRow(size_t index);
	bool changeColumnName(size_t index, const char* name);
	void filter(size_t index, const char* condStr, const char* value);

private:
	void expandCollection();

private:
	char* name;
	size_t colCount;
	size_t rowCount;
	size_t allocatedCapacity;
	char* delimiter;
	Column** columns;
	bool isEmpty;
	unsigned int width; //keeps the length of the longest value (out of every column), used to later display the table (spacings)
};