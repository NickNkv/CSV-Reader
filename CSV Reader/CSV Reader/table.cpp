#define	_CRT_SECURE_NO_WARNINGS
#include "table.hpp"
#include "utils.hpp"
#include <iostream>
#include <exception>
#include <stdexcept>
#include <cstring>
#include <fstream>

//when the table gets resized, (BONUS_CAPACITY) null pointers are added to this->columns
#define BONUS_CAPACITY 10 
#define MAX_LINE_LEN 1024
#define MAX_COLS 100

Table::Table() {
	this->name = nullptr;
	this->colCount = 0;
	this->rowCount = 0;
	this->allocatedCapacity = BONUS_CAPACITY;
	this->isEmpty = true;
	this->width = 0;

	this->delimiter = new (std::nothrow) char[2];
	if (!this->delimiter) {
		throw std::bad_alloc();
	}
	strcpy(this->delimiter, ",");

	this->columns = new (std::nothrow) Column*[this->allocatedCapacity];
	if (!this->columns) {
		delete[] this->delimiter;
		throw std::bad_alloc();
	}

	for (size_t i = 0; i < this->allocatedCapacity; i++) {
		this->columns[i] = nullptr;
	}
}

Table::Table(const char* name) {
	if (strlen(name) == 0) {
		throw std::invalid_argument("Name can not be empty string");
	}

	this->colCount = 0;
	this->rowCount = 0;
	this->isEmpty = true;
	this->width = 0;
	this->allocatedCapacity = BONUS_CAPACITY;

	this->name = new (std::nothrow) char[strlen(name) + 1];
	if (!this->name) {
		throw std::bad_alloc();
	}
	strcpy(this->name, name);

	this->delimiter = new (std::nothrow) char[2];
	if (!this->delimiter) {
		delete[] this->name;
		throw std::bad_alloc();
	}
	strcpy(this->delimiter, ",");

	this->columns = new (std::nothrow) Column * [this->allocatedCapacity];
	if (!this->columns) {
		delete[] this->name;
		delete[] this->delimiter;
		throw std::bad_alloc();
	}

	for (size_t i = 0; i < this->allocatedCapacity; i++) {
		this->columns[i] = nullptr;
	}
}

Table::Table(const Table& other) {
	this->colCount = other.colCount;
	this->rowCount = other.rowCount;
	this->isEmpty = other.isEmpty;
	this->width = other.width;
	this->allocatedCapacity = other.allocatedCapacity;

	this->name = new (std::nothrow) char[strlen(other.name) + 1];
	if (!this->name) {
		throw std::bad_alloc();
	}
	strcpy(this->name, other.name);

	this->delimiter = new (std::nothrow) char[strlen(other.delimiter) + 1];
	if (!this->delimiter) {
		delete[] this->name;
		throw std::bad_alloc();
	}
	strcpy(this->delimiter, other.delimiter);

	this->columns = new (std::nothrow) Column * [this->allocatedCapacity];
	if (!this->columns) {
		delete[] this->name;
		delete[] this->delimiter;
		throw std::bad_alloc();
	}

	for (size_t i = 0; i < this->allocatedCapacity; i++) {
		this->columns[i] = nullptr;
	}

	//other is guarantied to be in valid state (size <= allocatedCapacity) but I put this guard just in case
	if (this->colCount > this->allocatedCapacity) {
		std::cerr << "Dangerous situation in Table copy constructor: colCount > allocated capacity!\n";
		std::cerr << "Immediate investigation needed!" << std::endl;
		throw std::invalid_argument("Number of columns > allocated capacity!");
	}

	//deep copy of each col
	for (size_t i = 0; i < this->colCount; i++) {
		this->columns[i] = new (std::nothrow) Column(*other.columns[i]);
		if (!this->columns[i]) {
			for (size_t j = 0; j < i; j++) {
				delete this->columns[j];
			}
			delete[] this->columns;
			this->columns = nullptr;
			delete[] this->name;
			delete[] this->delimiter;
			throw std::bad_alloc();
		}
	}
}

Table::~Table() {
	delete[] this->name;
	delete[] this->delimiter;

	for (size_t i = 0; i < this->allocatedCapacity; i++) {
		delete this->columns[i];
	}
	delete[] this->columns;
}

Table& Table::operator=(const Table& other) {
	if (this == &other) {
		return *this;
	}

	//copying name in tempName
	char* tempName = new (std::nothrow) char[strlen(other.name) + 1];
	if (!tempName) {
		std::cerr << "Memory allocation error with operator =, try again!" << std::endl;
		return *this;
	}
	strcpy(tempName, other.name);

	//copying delimiter in tempDelimiter
	char* tempDelimiter = new (std::nothrow) char[strlen(other.delimiter) + 1];
	if (!tempDelimiter) {
		std::cerr << "Memory allocation error with operator =, try again!" << std::endl;
		delete[] tempName;
		return *this;
	}
	strcpy(tempDelimiter, other.delimiter);

	//copying the columns in tempColumns**
	Column** tempColumns = new (std::nothrow) Column*[other.allocatedCapacity];
	if (!tempColumns) {
		std::cerr << "Memory allocation error with operator =, try again!" << std::endl;
		delete[] tempName;
		delete[] tempDelimiter;
		return *this;
	}

	//other is guarantied to be in valid state (size <= allocatedCapacity) but I put this guard just in case
	if (other.colCount > other.allocatedCapacity) {
		std::cerr << "Dangerous situation in Table copy constructor: colCount > allocated capacity!\n";
		std::cerr << "Immediate investigation needed!" << std::endl;
		throw std::invalid_argument("Number of columns > allocated capacity!");
	}

	for (size_t i = 0; i < other.colCount; i++) {
		tempColumns[i] = new (std::nothrow) Column(*other.columns[i]);
		if (!tempColumns[i]) {
			for (size_t j = 0; j < i; j++) {
				delete tempColumns[i];
			}
			std::cerr << "Memory allocation error with operator =, try again!" << std::endl;
			delete[] tempColumns;
			delete[] tempName;
			delete[] tempDelimiter;
			return *this;
		}
	}

	//nullptr-ing the rest of the allocated memory
	for (size_t i = other.colCount; i < other.allocatedCapacity; i++) {
		tempColumns[i] = nullptr;
	}

	//point of no return - making changes to [this]
	for (size_t i = 0; i < this->allocatedCapacity; i++) {
		delete this->columns[i];
	}
	delete[] this->columns;
	this->columns = tempColumns;
	tempColumns = nullptr;

	delete[] this->name;
	this->name = tempName;
	tempName = nullptr;

	delete[] this->delimiter;
	this->delimiter = tempDelimiter;
	tempDelimiter = nullptr;

	this->colCount = other.colCount;
	this->rowCount = other.rowCount; 
	this->isEmpty = other.isEmpty;
	this->width = other.width;
	this->allocatedCapacity = other.allocatedCapacity;

	return *this;
}

//setters
void Table::setName(const char* name) {
	if (strlen(name) == 0) {
		std::cout << "Name can not be empty! Try again!" << std::endl;
		return;
	}

	char* tempName = new (std::nothrow) char[strlen(name) + 1];
	if (!tempName) {
		std::cerr << "Memory allocation error while changing table " << this->name << "'s name\n";
		std::cerr << "Try again!" << std::endl;
		return;
	}

	strcpy(tempName, name);
	this->name = tempName;
	tempName = nullptr;
}

void Table::setDelimiter(const char* delimiter) {
	if (strlen(delimiter) == 0) {
		std::cout << "Delimiter can not be empty! Try again!" << std::endl;
		return;
	}

	char* tempDel = new (std::nothrow) char[strlen(delimiter) + 1];
	if (!tempDel) {
		std::cerr << "Memory allocation error while changing table " << this->name << "'s delimiter\n";
		std::cerr << "Try again!" << std::endl;
		return;
	}

	strcpy(tempDel, delimiter);
	this->delimiter = tempDel;
	tempDel = nullptr;
}

const Cell* Table::getCellAt(size_t row, size_t col) {
	if (row < 0 || row >= this->rowCount || col < 0 || col >= this->colCount) {
		std::cout << "Invalid index!\n";
		return nullptr;
	}
	return this->columns[col]->getCellAt(row);
}

bool Table::addColumn(Column& column) {
	//if allocated space is NOT enough for one more column
	if (this->colCount + 1 > this->allocatedCapacity) {
		expandCollection();
	}

	this->columns[this->colCount] = new (std::nothrow) Column(column);
	if (!this->columns[this->colCount]) {
		std::cerr << "Memory allocation error while adding new column! Try again!" << std::endl;
		return false;
	}

	this->colCount++;
	return true;
}

void Table::removeColumnAt(size_t index) {
	if (index < 0 || index >= this->colCount) {
		std::cout << "Invalid index for column removal!\n";
		return;
	}

	//when we delete a column, we move the rest of the columns one space backwards and nullptr the last column
	delete this->columns[index];
	for (size_t i = index; i < this->colCount - 1; i++) {
		this->columns[i] = this->columns[i + 1];
	}

	this->columns[this->colCount - 1] = nullptr;

	this->colCount -= 1;
}

bool Table::changeColumnOrder(unsigned* order, unsigned size) {
	if (this->colCount != size) {
		std::cout << "Not complete set of positions!\n\n";
		return false;
	}

	bool* seen = new (std::nothrow) bool[size + 1];
	if (!seen) {
		std::cout << "Memory allocation error!\n\n";
		return false;
	}

	for (unsigned i = 0; i < size; i++) {
		if (order[i] < 1 || order[i] > this->colCount) {
			delete[] seen;
			std::cout << "Not complete set of positions!\n\n";
			return false;
		}
		seen[order[i]] = true;
	}

	//check if all numbers from one to colCount are present
	for (unsigned i = 1; i <= size; i++) {
		if (!seen[i]) {
			delete[] seen;
			std::cout << "Not complete set of positions!\n\n";
			return false;
		}
	}

	//change of columns order
	Column** newCols = new (std::nothrow) Column * [size];
	if (!newCols) {
		std::cout << "Memory allocation error!\n\n";
		return false;
	}
	for (size_t i = 0; i < size; i++) {
		size_t index = order[i] - 1;
		newCols[i] = this->columns[index];
	}

	delete[] this->columns;
	this->columns = newCols;

	delete[] seen;
	return true;
}

void Table::expandCollection() {
	Column** tempCollection = new (std::nothrow) Column * [this->colCount + 1 + BONUS_CAPACITY];
	if (!tempCollection) {
		throw std::bad_alloc();
	}

	//copying the columns pointers
	for (size_t i = 0; i < this->colCount; i++) {
		tempCollection[i] = this->columns[i];
	}

	//nullptr-ing the rest of the allocated memory
	for (size_t i = this->colCount; i < this->colCount + 1 + BONUS_CAPACITY; i++) {
		tempCollection[i] = nullptr;
	}

	delete[] this->columns;
	this->columns = tempCollection;
	this->allocatedCapacity = this->colCount + 1 + BONUS_CAPACITY;

	//std::cout << "Collection expanded\n";
}

//mechanics
size_t parceCSVLine(char* line, char** fields, char delimiter = ',') {
	size_t count = 0;
	char* token = strtok(line, &delimiter);
	while (token != nullptr && count < MAX_COLS) {
		fields[count++] = token;
		token = strtok(nullptr, &delimiter);
	}
	return count;
}

bool Table::populateTable(const char* fileName) {
	//check if table is empty/populated
	if (!isEmpty) {
		std::cout << "This table is not empty!\n"
			<< "Empty the table first, then fill with data!\n";
		return false;
	}

	//open file
	std::ifstream file(fileName);
	if (!file.is_open()) {
		std::cerr << "Failed to open file.\n";
		std::cerr << "Check the file name.\n";
		return false;
	}

	char line[MAX_LINE_LEN];
	char* data[MAX_COLS]; //col types

	// 1 - get column types
	file.getline(line, MAX_LINE_LEN);
	size_t colNum = parceCSVLine(line, data);

	// 2 - alocate cols if needed
	if (this->allocatedCapacity < colNum) {
		Column** temp = new (std::nothrow) Column * [colNum + BONUS_CAPACITY];
		if (!temp) {
			std::cout << "Memory allocation error, try again!\n";
			file.close();
			//for (int i = 0; i < MAX_COLS; i++) {
			//	delete[] types[i];
			//	delete[] names[i];
			//}
			return false;
		}

		delete[] this->columns;
		this->columns = temp;
		temp = nullptr;
		this->allocatedCapacity = colNum + BONUS_CAPACITY;
	}
	this->colCount = colNum;

	// 3 - create cols
	for (size_t i = 0; i < this->colCount; i++) {
		this->columns[i] = new (std::nothrow) Column(util::strToColumnType(data[i]), "temp");
		if (!this->columns[i]) {
			for (size_t j = 0; j < i; j++) {
				delete this->columns[j];
			}

			std::cout << "Memory allocation error, try again!\n";
			file.close();
			return false;
		}
	}

	// 4 - get column names
	file.getline(line, MAX_LINE_LEN);
	size_t nameNum = parceCSVLine(line, data);

	for (size_t i = 0; i < this->colCount; i++) {
		this->columns[i]->setName(data[i]);
	}

	// 5 - save data
	while (file.getline(line, MAX_LINE_LEN)) {
		size_t count = parceCSVLine(line, data);
		if (count != colNum) {
			std::cout << "Column count mismatch!\n";
			continue;
		}
		for (size_t i = 0; i < colNum; i++) {
			try {
				Cell temp(data[i]);
				bool flag = this->columns[i]->addCell(temp);

				if (!flag) throw std::bad_alloc();
			}
			catch (const std::exception& e) {
				std::cerr << "Exception: " << e.what() << "\n";
				file.close();
				//for (size_t i = 0; i < MAX_COLS; i++) {
				//	delete[] names[i];
				//}
				return false;
			}
		}
		this->rowCount += 1;
	}

	// *6 - set the width
	for (size_t i = 0; i < this->colCount; i++) {
		if (this->columns[i]->getWidth() > this->width) {
			this->width = this->columns[i]->getWidth();
		}
	}

	this->isEmpty = false;
	file.close();
	return true;
}

bool Table::saveToFile(const char* fileName) {
	std::ofstream out(fileName);
	if (!out.is_open()) {
		return false;
	}

	// 1 - column types
	for (size_t i = 0; i < this->colCount; i++) {
		out << util::columnTypeToStr(this->columns[i]->getType());
		if (i < this->colCount - 1) out << ",";
	}
	out << '\n';

	// 2 - column names
	for (size_t i = 0; i < this->colCount; i++) {
		out << this->columns[i]->getName();
		if (i < this->colCount - 1) out << ",";
	}
	out << '\n';

	// 3 - data
	for (size_t i = 0; i < this->rowCount; i++) {
		for (size_t j = 0; j < this->colCount; j++) {
			out << this->columns[j]->getCellAt(i)->getValue();
			if (j < this->colCount - 1) out << ",";
		}
		out << '\n';
	}

	out.close();
	return true;
}

void Table::printTable() {
	// data types
	std::cout << "   ";
	for (size_t i = 0; i < this->colCount; i++) {
		std::cout << util::columnTypeToStr(this->columns[i]->getType());
		if (i < this->colCount - 1) std::cout << this->delimiter;
	}
	std::cout << std::endl;

	// column names
	std::cout << "   ";
	for (size_t i = 0; i < this->colCount; i++) {
		std::cout << this->columns[i]->getName();
		if (i < this->colCount - 1) std::cout << this->delimiter;
	}
	std::cout << std::endl;

	// data
	for (size_t i = 0; i < this->rowCount; i++) {
		std::cout << i + 1 << " | ";
		for (size_t j = 0; j < this->colCount; j++) {
			std::cout << this->columns[j]->getCellAt(i)->getValue();
			if (j < this->colCount - 1) std::cout << this->delimiter;
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
}

bool Table::sort(size_t index, bool ascending) {
	if (index < 0 || index >= this->colCount) {
		return false;
	}

	bool result = true;
	for (size_t i = 0; i < this->rowCount - 1; i++) {
		for (size_t j = i + 1; j < this->rowCount; j++) {
			int cmp = util::compareCells(this->columns[index]->getCellAt(i), this->columns[index]->getCellAt(j), this->columns[index]->getType());

			if ((ascending && cmp > 0) || (!ascending && cmp < 0)) {
				result = swapRows(i, j);
			}
		}
	}

	return result;
}

long long Table::findColByName(const char* name) {
	for (size_t i = 0; i < this->colCount; i++) {
		if (strcmp(name, this->columns[i]->getName()) == 0) {
			return i;
		}
	}
	
	//name not found
	return -1;
}

bool Table::swapRows(size_t first, size_t second) {
	if (first < 0 || second < 0 || first >= this->rowCount || second >= this->rowCount) {
		return false;
	}

	bool result = false;
	for (size_t i = 0; i < this->colCount; i++) {
		result = this->columns[i]->swapTwoCells(first, second);
	}

	return result;
}

void Table::emptyTable() {
	for (size_t i = 0; i < this->colCount; i++) {
		for (size_t j = this->rowCount - 1; j > 0; j--) {
			this->columns[i]->removeCellAt(j);
		}
	}
	this->rowCount = 0;
	this->colCount = 0;
	this->width = 0;
	this->isEmpty = true;
}

bool Table::rowsEqual(size_t first, size_t second) {
	if (first < 0 || second < 0 || first >= this->rowCount || second >= this->rowCount) {
		std::cout << "Invalid index for comparing rows!\n";
		return false;
	}

	for (size_t i = 0; i < this->colCount; i++) {
		const char* valA = this->columns[i]->getCellAt(first)->getValue();
		const char* valB = this->columns[i]->getCellAt(second)->getValue();
		if (strcmp(valA, valB) != 0) return false;
	}

	return true;
}

void Table::removeIdenticalRows() {
	for (size_t i = 0; i < this->rowCount; i++) {
		for (size_t j = i + 1; j < this->rowCount; ) {
			if (rowsEqual(i, j)) {
				removeRow(j);
			}
			else {
				j++;
			}
		}
	}
}

void Table::removeRow(size_t index) {
	if (index < 0 || index >= this->rowCount) {
		std::cout << "Invalid index for removal of a row\n";
		return;
	}

	for (size_t i = 0; i < this->colCount; i++) {
		this->columns[i]->removeCellAt(index);
	}
	this->rowCount -= 1;
}

bool Table::changeColumnName(size_t index, const char* name) {
	if (index < 0 || index >= this->colCount) {
		return false;
	}

	return this->columns[index]->setName(name);
}

void Table::filter(size_t index, const char* condStr, const char* value) {
	if (index < 0 || index >= this->colCount) {
		std::cout << "Invalid index for removal of a row\n";
		return;
	}

	Condition cond = util::strToCondition(condStr);
	ColumnType type = this->columns[index]->getType();

	for (size_t i = 0; i < this->rowCount; ) {
		const char* cellVal = this->columns[index]->getCellAt(i)->getValue();
		if (!util::filterCompare(cellVal, value, cond, type)) {
			removeRow(i);
		}
		else {
			i++;
		}
	}
}

//const char* Table::generateUniqueColumnName(const char* initName) {
//	char buffer[128];
//	int suffix = 1;
//
//	while (true) {
//		snprintf(buffer, sizeof(buffer), "%s_%d", initName, suffix);
//
//		bool exists = false;
//		for (int i = 0; i < this->colCount; i++) {
//			if (strcmp(columns[i]->getName(), buffer) == 0) {
//				exists = true;
//				break;
//			}
//		}
//
//		if (!exists) break;
//		suffix++;
//	}
//
//	return buffer;
//}


bool Table::duplicateColumn(size_t index) {
	if (index < 0 || index >= this->colCount) {
		std::cout << "Invalid index for removal of a row\n";
		return false;
	}

	//is allocated space is not enough for one more column
	if (this->colCount + 1 > this->allocatedCapacity) {
		try {
			expandCollection();
		}
		catch (std::exception& e) {
			std::cout << e.what() << "\n";
			return false;
		}
	}

	//const char* newName = generateUniqueColumnName(this->columns[index]->getName());

	Column* duplicate = new Column(*this->columns[index]);

	this->columns[this->colCount] = duplicate;
	this->colCount++;

	return true;
}

void Table::addExtremeValues(bool min) {
	for (size_t i = 0; i < this->colCount; i++) {
		size_t targetIndex = 0;
		for (size_t j = 1; j < this->rowCount; j++) {
			int cmp = util::compareCells(this->columns[i]->getCellAt(targetIndex), this->columns[i]->getCellAt(j), this->columns[i]->getType());

			if ((min && cmp > 0) || (!min && cmp < 0)) {
				targetIndex = j;
			}
		}
		Cell temp(this->columns[i]->getCellAt(targetIndex)->getValue());
		this->columns[i]->addCell(temp);
	}

	this->rowCount++;
}
