#pragma once
#include "types.hpp"


namespace util {
	const char* columnTypeToStr(ColumnType type);
	ColumnType detectType(const char* value);
}