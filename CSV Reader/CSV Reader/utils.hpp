#pragma once
#include "types.hpp"
#include "conditions.hpp"
#include "cell.hpp"


namespace util {
	const char* columnTypeToStr(ColumnType type);
	ColumnType strToColumnType(const char* str);
	ColumnType detectType(const char* value);
	bool isNum(const char* value);
	int compareCells(const Cell* a, const Cell* b, ColumnType type);
	bool filterCompare(const char* lhs, const char* rhs, Condition cond, ColumnType type);
	Condition strToCondition(const char* cond);
	void clearInputBuffer();
	void printPadded(const char* value, unsigned width);
}