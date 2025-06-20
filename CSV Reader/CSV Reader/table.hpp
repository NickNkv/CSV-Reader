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
	void removeColumnAt(size_t index);
	void printTable(int code = 0);
	void emptyTable();
	void removeIdenticalRows();
	void removeRow(size_t index);
	void filter(size_t index, const char* condStr, const char* value);
	void addExtremeValues(bool min);
	void copyRow(size_t index);
	bool changeColumnOrder(unsigned* order, unsigned size);
	bool addColumn(Column& column);
	bool populateTable(const char* fileName);
	bool saveToFile(const char* fileName);
	bool sort(size_t index, bool ascending = true);
	bool swapRows(size_t first, size_t second); //indexes of the two rows
	bool rowsEqual(size_t first, size_t second);
	bool changeColumnName(size_t index, const char* name);
	bool duplicateColumn(size_t index);
	bool editCell(size_t col, size_t cell, const char* newValue);
	bool addRow();
	long long findColByName(const char* name);
	

private:
	void expandCollection();
	const char* generateUniqueColumnName(const char* initName);

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