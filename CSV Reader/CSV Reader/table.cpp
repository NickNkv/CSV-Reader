#define	_CRT_SECURE_NO_WARNINGS
#include "table.hpp"
#include <iostream>
#include <exception>

//when the table gets resized, (BONUS_CAPACITY) null pointers are added to this->columns
#define BONUS_CAPACITY 10 

Table::Table() {
	this->name = nullptr;
	this->colCount = 0;
	this->rowCount = 0;
	this->allocatedCapacity = BONUS_CAPACITY;

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
