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

//mechanics
void Table::addColumn(Column& column) {

}

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
	}

	//open file
	std::ifstream file(fileName);
	if (!file.is_open()) {
		std::cerr << "Failed to open file.\n";
		std::cerr << "Check the file name.\n";
		return false;
	}

	char line[MAX_LINE_LEN];
	char* types[MAX_COLS]; //col types
	char* names[MAX_COLS]; //col names

	// 1 - get column types
	file.getline(line, MAX_LINE_LEN);
	size_t colNum = parceCSVLine(line, types);

	// 2 - get column names
	file.getline(line, MAX_LINE_LEN);
	size_t nameNum = parceCSVLine(line, names);

	// 3 - alocate cols if needed
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

	// 4 - create cols
	for (size_t i = 0; i < this->colCount; i++) {
		this->columns[i] = new (std::nothrow) Column(util::strToColumnType(types[i]), names[i]);
		if (!this->columns[i]) {
			for (size_t j = 0; j < i; j++) {
				delete this->columns[j];
			}

			std::cout << "Memory allocation error, try again!\n";
			file.close();
			//for (size_t i = 0; i < MAX_COLS; i++) {
			//	delete[] types[i];
			//	delete[] names[i];
			//}
			return false;
		}
	}

	//types is no longer needed, we will use names to populate the data
	/*for (size_t i = 0; i < MAX_COLS; i++) {
		delete[] types[i];
	}*/

	// 5 - save data
	while (file.getline(line, MAX_LINE_LEN)) {
		size_t count = parceCSVLine(line, names);
		if (count != colNum) {
			std::cout << "Column count mismatch!\n";
			continue;
		}
		for (size_t i = 0; i < colNum; i++) {
			try {
				Cell temp(names[i]);
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
		
	return true;
}
