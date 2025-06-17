#pragma once
#include "types.hpp"


namespace util {
	const char* columnTypeToStr(ColumnType type);
	ColumnType strToColumnType(const char* str);
	ColumnType detectType(const char* value);
}