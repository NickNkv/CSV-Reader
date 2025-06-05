#include "utils.hpp"

const char* utilGnrl::columnTypeToStr(ColumnType type) {
	switch (type) {
	case ColumnType::Text: return "Text";
		break;
	case ColumnType::Number: return "Number";
		break;
	case ColumnType::Currency: return "Currency";
		break;
	case ColumnType::EGN: return "EGN";
		break;
	case ColumnType::FacultyNumber: return "Faculty number";
		break;
	default: return "Unknown column type";
		break;
	}
}