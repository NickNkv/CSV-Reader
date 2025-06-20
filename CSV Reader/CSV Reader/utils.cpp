#define _CRT_SECURE_NO_WARNINGS
#include "utils.hpp"
#include <cstring>
#include <cstdlib>

bool isNumber(const char* value) {
	if (!value || *value == '\0') {
		return false;
	}

	int i = 0;

	// Check for sign
	if (value[i] == '-' || value[i] == '+') {
		i++;
	}

	bool hasDigits = false;
	for (; value[i]; ++i) {
		if (value[i] >= '0' && value[i] <= '9') {
			hasDigits = true;
		}
		else if (value[i] == '.' || value[i] == ',') {
			continue;
		}
		else {
			return false;
		}
	}

	return hasDigits;
}

//the method for EGN checking was taken from wikipedia
bool isValidEGN(const char* value) {
	if (strlen(value) != 10 || !isNumber(value)) return false;

	int year = (value[0] - '0') * 10 + (value[1] - '0');
	int month = (value[2] - '0') * 10 + (value[3] - '0');
	int day = (value[4] - '0') * 10 + (value[5] - '0');

	//date check
	if (month >= 1 && month <= 12) {
		year += 1900;
	}
	else if (month >= 21 && month <= 32) {
		year += 1800;
		month -= 20;
	}
	else if (month >= 41 && month <= 52) {
		year += 2000;
		month -= 40;
	}
	else {
		return false;
	}

	if (day < 1 || day > 31 || month < 1 || month > 12) return false;

	//controll number check
	int coeficients[9] = { 2, 4, 8, 5, 10, 9, 7, 3, 6 };
	int sum = 0;
	for (int i = 0; i < 9; i++) {
		sum += (value[i] - '0') * coeficients[i];
	}

	int remainder = sum % 11;
	int checkDigit = remainder < 10 ? remainder : 0;

	if (checkDigit == (value[9] - '0')) {
		return true;
	}
	else {
		return false;
	}
}

bool isOldFacultyNumber(const char* value) {
	size_t size = strlen(value);
	if (size != 5) return false;

	for (int i = 0; i < 5; i++) {
		if (value[i] < '0' || value[i] > '9') return false;
	}

	return true;
}

bool isNewFacultyNumber(const char* value) {
	size_t size = strlen(value);
	if (size != 10) return false;
	
	//first digit
	if (value[0] < '0' || value[0] > '9') return false;

	//Letter code
	if (value[1] < 'A' || value[1] > 'Z') return false;
	if (value[2] < 'A' || value[2] > 'Z') return false;

	for (int i = 3; i < 10; i++) {
		if (value[i] < '0' || value[i] > '9') return false;
	}

	return true;
}

bool isCurrency(const char* value) {
	size_t size = strlen(value);
	if (size < 4) {
		return false;
	}

	//checking the first and last digit
	if (value[0] < '0' || value[0] > '9') return false;
	if (value[size - 4] < '0' || value[size - 4] > '9') return false;

	for (size_t i = 1; i < size - 4; i++) {
		if (value[i] < '0' || value[i] > '9') {
			if (value[i] != '.' && value[i] != ',') {
				return false;
			}
		}
	}

	return true;
}

bool isQuotedText(const char* value) {
	if (strlen(value) < 2) {
		return false;
	}
	if (value[0] == '"' && value[strlen(value) - 1] == '"') {
		return true;
	}
	else if (value[0] == '\'' && value[strlen(value) - 1] == '\'') {
		return true;
	}
	else {
		return false;
	}
}


// util general
const char* util::columnTypeToStr(ColumnType type) {
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

ColumnType util::strToColumnType(const char* str) {
	if (strcmp(str, "Text") == 0) return ColumnType::Text;
	if (strcmp(str, "Number") == 0) return ColumnType::Number;
	if (strcmp(str, "Currency") == 0) return ColumnType::Currency;
	if (strcmp(str, "EGN") == 0) return ColumnType::EGN;
	if (strcmp(str, "Faculty number") == 0) return ColumnType::FacultyNumber;
	return ColumnType::Text;
}

ColumnType util::detectType(const char* value) {
	if (isQuotedText(value)) return ColumnType::Text;
	if (isValidEGN(value)) return ColumnType::EGN;
	if (isOldFacultyNumber(value)) return ColumnType::FacultyNumber;
	if (isNewFacultyNumber(value)) return ColumnType::FacultyNumber;
	if (isCurrency(value)) return ColumnType::Currency;
	if (isNumber(value)) return ColumnType::Number;
	return ColumnType::Text;
}

bool util::isNum(const char* value) {
	return isNumber(value);
}

int util::compareCells(const Cell* a, const Cell* b, ColumnType type) {
	switch (type) {
	case ColumnType::Number: {
		double numA = atof(a->getValue());
		double numB = atof(b->getValue());

		return numA < numB ? -1 : (numA > numB ? 1 : 0);
	}
	case ColumnType::Currency: {
		size_t lenA = strlen(a->getValue());
		size_t lenB = strlen(b->getValue());
		if (lenA < 4 || lenB < 4) {
			return 0;
		}

		char numStrA[64];
		char numStrB[64];
		strncpy(numStrA, a->getValue(), lenA - 3);
		numStrA[lenA - 3] = '\0';

		strncpy(numStrB, b->getValue(), lenB - 3);
		numStrB[lenB - 3] = '\0';

		double numA = atof(numStrA);
		double numB = atof(numStrB);

		return numA < numB ? -1 : (numA > numB ? 1 : 0);
	}
	case ColumnType::Text:
		return strcmp(a->getValue(), b->getValue());
	case ColumnType::EGN: {
		int yearA = (a->getValue()[0] - '0') * 10 + (a->getValue()[1] - '0');
		int monthA = (a->getValue()[2] - '0') * 10 + (a->getValue()[3] - '0');
		int dayA = (a->getValue()[4] - '0') * 10 + (a->getValue()[5] - '0');

		//date check
		if (monthA >= 1 && monthA <= 12) {
			yearA += 1900;
		}
		else if (monthA >= 21 && monthA <= 32) {
			yearA += 1800;
			monthA -= 20;
		}
		else if (monthA >= 41 && monthA <= 52) {
			yearA += 2000;
			monthA -= 40;
		}
		else {
			return false;
		}

		int yearB = (b->getValue()[0] - '0') * 10 + (b->getValue()[1] - '0');
		int monthB = (b->getValue()[2] - '0') * 10 + (b->getValue()[3] - '0');
		int dayB = (b->getValue()[4] - '0') * 10 + (b->getValue()[5] - '0');

		//date check
		if (monthB >= 1 && monthB <= 12) {
			yearB += 1900;
		}
		else if (monthB >= 21 && monthB <= 32) {
			yearB += 1800;
			monthB -= 20;
		}
		else if (monthB >= 41 && monthB <= 52) {
			yearB += 2000;
			monthB -= 40;
		}
		else {
			return false;
		}

		if (yearA < yearB) return -1;
		else if (yearA > yearB) return 1;
		else {
			if (monthA < monthB) return -1;
			else if (monthA > monthB) return 1;
			else {
				if (dayA < dayB) return -1;
				else if (dayA > dayB) return 1;
				else return 0;
			}
		}
	}
	case ColumnType::FacultyNumber: {
		return strcmp(a->getValue(), b->getValue());
	}
	default:
		return 0;
	}
}


