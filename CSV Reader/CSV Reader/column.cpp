#define	_CRT_SECURE_NO_WARNINGS
#include "column.hpp"
#include <iostream>
#include <exception>

//when the column gets resized, (BONUS_CAPACITY) null pointers are added to this->cells
#define BONUS_CAPACITY 10 

//lifecycle
Column::Column(size_t size, ColumnType type, const char* name) {
	this->size = size;
	this->allocatedCapacity = size + BONUS_CAPACITY;
	this->type = type;

	if (strlen(name) == 0) {
		throw std::invalid_argument("Name can not be empty string");
	}
	this->name = new (std::nothrow) char[strlen(name) + 1];
	if (!this->name) {
		throw std::bad_alloc();
	}
	strcpy(this->name, name);

	this->cells = new (std::nothrow) Cell*[this->allocatedCapacity];
	if (!this->cells) {
		delete[] this->name;
		throw std::bad_alloc();
	}

	for (size_t i = 0; i < this->allocatedCapacity; i++) {
		this->cells[i] = nullptr;
	}
}

Column::Column(const Column& other) {
	this->size = other.size;
	this->allocatedCapacity = other.allocatedCapacity; 
	this->type = other.type;

	this->name = new (std::nothrow) char[strlen(other.name) + 1];
	if (!this->name) {
		throw std::bad_alloc();
	}
	strcpy(this->name, other.name);

	this->cells = new (std::nothrow) Cell*[this->allocatedCapacity];
	if (!this->cells) {
		delete[] this->name;
		throw std::bad_alloc();
	}

	for (size_t i = 0; i < this->allocatedCapacity; i++) {
		this->cells[i] = nullptr;
	}

	//other is guarantied to be in valid state (size <= allocatedCapacity) but I put this guard just in case
	if (this->size > this->allocatedCapacity) {
		std::cerr << "Dangerous situation in Column copy constructor: size > allocated capacity!\n";
		std::cerr << "Immediate investigation needed!" << std::endl;
		throw std::invalid_argument("size > allocated capacity!"); 
	}

	//deep copy of each cell
	for (size_t i = 0; i < this->size; i++) {
		this->cells[i] = new (std::nothrow) Cell(*other.cells[i]);
		if (!this->cells[i]) {
			for (size_t j = 0; j < i; j++) {
				delete this->cells[i];
			}
			delete[] this->cells;
			this->cells = nullptr;
			delete[] this->name;
			throw std::bad_alloc();
		}
	}
}

Column::~Column() {
	delete[] this->name;

	for (size_t i = 0; i < this->allocatedCapacity; i++) {
		delete this->cells[i];
	}
	delete[] this->cells;
}

//mechanics
void Column::setType() {
	if (this->size != 0) {
		std::cout << "The column is not empty and thus can not have its data type changed!" << std::endl;
		return;
	}

	int option = 0;
	do {
		std::cout 
				<< "Choose data type:\n"
				<< "1 - Text\n"
				<< "2 - Number\n"
				<< "3 - Currency\n"
				<< "4 - EGN\n"
				<< "5 - FacultyNumber"
				<< "6 - Cancel"
				<< std::endl;
		
		std::cout << "Option: ";
		std::cin >> option;
		if (option < 1 || option > 6) {
			std::cout << "Invalid option!\n" << std::endl;
		}
	} while (option < 1 || option > 6);

	switch (option) {
	case 1: this->type = ColumnType::Text;
		break;
	case 2: this->type = ColumnType::Number;
		break;
	case 3: this->type = ColumnType::Currency;
		break;
	case 4: this->type = ColumnType::EGN;
		break;
	case 5: this->type = ColumnType::FacultyNumber;
		break;
	default: 
		break;
	}
}
