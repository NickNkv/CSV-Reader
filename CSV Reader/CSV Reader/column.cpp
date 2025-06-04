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

	//deep copy of each cell
	for (size_t i = 0; i < this->size; i++) {

	}
}
