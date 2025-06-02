#include "cell.hpp"

//lifecycle
Cell::Cell() {
	this->value = nullptr;
	this->dataType = ColumnType::Text;
}

Cell::Cell(const char* value, ColumnType dataType)
{
}

//setters
