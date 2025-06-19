#pragma once
#include "types.hpp"
#include "cell.hpp"


namespace util {
	const char* columnTypeToStr(ColumnType type);
	ColumnType strToColumnType(const char* str);
	ColumnType detectType(const char* value);
	bool isNum(const char* value);
	int compareCells(const Cell* a, const Cell* b, ColumnType type);
}