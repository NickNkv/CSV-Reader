#define _CRT_SECURE_NO_WARNINGS
#include "cell.hpp"
#include <iostream>
#include <cstring>

//lifecycle
Cell::Cell() {
	this->value = nullptr;
}

Cell::Cell(const char* value) {
	if (strlen(value) == 0) {
		this->value = nullptr;
	}
	else {
		this->value = new (std::nothrow) char[strlen(value) + 1];
		if (!this->value) {
			throw std::bad_alloc();
		}

		strcpy(this->value, value);
	}
}

Cell::Cell(const Cell& other) {
	if (!other.value) {
		this->value = nullptr;
	}
	else {
		this->value = new (std::nothrow) char[strlen(other.value) + 1];
		if (!this->value) {
			throw std::bad_alloc();
		}

		strcpy(this->value, other.value);
	}
}

Cell::~Cell() {
	delete[] this->value;
}

Cell& Cell::operator=(const Cell& other) {
	if (this == &other) {
		return *this;
	}

	char* temp = new (std::nothrow) char[strlen(other.value) + 1];
	if (!temp) {
		std::cout << "Memory allocation error, please try again!" << std::endl;
	}
	else {
		strcpy(temp, other.value);
		delete[] this->value;
		this->value = temp;
		temp = nullptr;
	}

	return *this;
}

//setters

void Cell::setValue(const char* value) {
	if (strlen(value) == 0) {
		delete[] this->value;
		this->value = nullptr;
	}
	else {
		char* temp = new (std::nothrow) char[strlen(value) + 1];
		if (!temp) {
			std::cout << "Memory allocation error, please try again!" << std::endl;
			return;
		}

		strcpy(temp, value);
		delete[] this->value;
		this->value = temp;
		temp = nullptr;
	}
}
