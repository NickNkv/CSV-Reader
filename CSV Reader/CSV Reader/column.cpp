#define	_CRT_SECURE_NO_WARNINGS
#include "column.hpp"
#include "utils.hpp"
#include <iostream>
#include <exception>

//when the column gets resized, (BONUS_CAPACITY) null pointers are added to this->cells
#define BONUS_CAPACITY 10 

Column::Column() {
	this->size = 0;
	this->allocatedCapacity = BONUS_CAPACITY;
	this->type = ColumnType::Text;
	this->width = strlen(util::columnTypeToStr(this->type));
	this->name = nullptr;

	this->cells = new (std::nothrow) Cell*[this->allocatedCapacity];
	if (!this->cells) {
		throw std::bad_alloc();
	}

	for (size_t i = 0; i < this->allocatedCapacity; i++) {
		this->cells[i] = nullptr;
	}
}

//lifecycle
Column::Column(ColumnType type, const char* name) {
	this->size = 0;
	this->allocatedCapacity = BONUS_CAPACITY;
	this->type = type;
	this->width = strlen(util::columnTypeToStr(this->type));;

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
	this->width = other.width;

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
	//std::cout << "Size: " << this->size << "  Alloc: " << this->allocatedCapacity << "\n";
	delete[] this->name;

	for (size_t i = 0; i < this->allocatedCapacity; i++) {
		delete this->cells[i];
	}
	delete[] this->cells;
}

Column& Column::operator=(const Column& other) {
	if (this == &other) {
		return *this;
	}

	//copying the name in temp name
	char* tempName = new (std::nothrow) char[strlen(other.name) + 1];
	if (!tempName) {
		std::cerr << "Memory allocation error with operator =, try again!" << std::endl;
		return *this;
	}
	strcpy(tempName, other.name);

	//copying the cells in a temp** container
	Cell** tempCells = new (std::nothrow) Cell*[other.allocatedCapacity];
	if (!tempCells) {
		std::cerr << "Memory allocation error with operator =, try again!" << std::endl;
		delete[] tempName;
		return *this;
	}

	//other is guarantied to be in valid state (size <= allocatedCapacity) but I put this guard just in case
	if (other.size > other.allocatedCapacity) {
		std::cerr << "Dangerous situation in Column operator= : other.size > other.allocated capacity!\n";
		std::cerr << "Immediate investigation needed!" << std::endl;
		throw std::invalid_argument("other.size > other.allocated capacity!");
	}

	for (size_t i = 0; i < other.size; i++) {
		tempCells[i] = new (std::nothrow) Cell(*other.cells[i]);
		if (!tempCells[i]) {
			for (size_t j = 0; j < i; j++) {
				delete tempCells[i];
			}
			delete[] tempCells;
			delete[] tempName;
			std::cerr << "Memory allocation error with operator =, try again!" << std::endl;
			return *this;
		}
	}
	//nullptr-ing the rest of the allocated memory
	for (size_t i = other.size; i < other.allocatedCapacity; i++) {
		tempCells[i] = nullptr;
	}

	//point of no return - making changes to [this]
	for (size_t i = 0; i < this->allocatedCapacity; i++) {
		delete this->cells[i];
	}
	delete[] this->cells;

	this->cells = tempCells;
	tempCells = nullptr;

	delete[] this->name;
	this->name = tempName;
	tempName = nullptr;

	this->size = other.size;
	this->allocatedCapacity = other.allocatedCapacity;
	this->type = other.type;
	this->width = other.width;

	return *this;
}

Cell* Column::operator[](size_t index) {
	if (index < 0 || index >= this->size) {
		std::cout << "Invalid index!\n";
		std::cout << "For this column valid index would be in range [0, " << this->size - 1 << "]" << std::endl;
		return nullptr;
	}
	return this->cells[index];
}

const Cell* Column::operator[](size_t index) const {
	if (index < 0 || index >= this->size) {
		std::cout << "Invalid index!\n";
		std::cout << "For this column valid index would be in range [0, " << this->size - 1 << "]" << std::endl;
		return nullptr;
	}
	return this->cells[index];
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
				<< "5 - Faculty Number\n"
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

	//change the width
	unsigned tempWidth = strlen(util::columnTypeToStr(this->type));
	if (tempWidth > this->width) {
		this->width = tempWidth;
	}
}

bool Column::setName(const char* name) {
	if (strlen(name) == 0) {
		std::cout << "Name can not be empty! Try again!" << std::endl;
		return false;
	}

	char* tempName = new (std::nothrow) char[strlen(name) + 1];
	if (!tempName) {
		std::cerr << "Memory allocation error while changing column " << this->name << "'s name\n";
		std::cerr << "Try again!" << std::endl;
		return false;
	}

	strcpy(tempName, name);
	this->name = tempName;
	tempName = nullptr;

	//change the width
	if (strlen(this->name) > this->width) {
		this->width = strlen(this->name);
	}

	return true;
}

bool Column::insertCellAt(size_t index, Cell& cell) {
	if (this->size == 0) {
		std::cout << "Size is 0, use addCell() method!" << std::endl;
		return false;
	}

	if (index < 0 || index >= this->size) {
		std::cout << "Invalid index!\n";
		std::cout << "For this column valid index would be in range [0, " << this->size - 1 << "]" << std::endl;
		return false;
	}

	//if allocated space is NOT enough for one more cell
	if (this->size + 1 > this->allocatedCapacity) {
		expandCollection();
	}

	this->cells[this->size] = new (std::nothrow) Cell();
	if (!this->cells[this->size]) {
		std::cerr << "Memory allocation error when inserting a cell at index " << index << "!" << std::endl;
		std::cerr << "Try again!" << std::endl;
		return false;
	}

	for (size_t i = this->size; i > index; i--) {
		this->cells[i] = this->cells[i - 1];
	}
	this->cells[index] = new (std::nothrow) Cell(cell);


	//change the width
	if (cell.getValue()) {
		if (this->width < strlen(cell.getValue())) {
			this->width = strlen(cell.getValue());
		}
	}

	this->size += 1;
	return true;
}

bool Column::addCell(Cell& cell) {
	//if allocated space is NOT enough for one more cell
	if (this->size + 1 > this->allocatedCapacity) {
		expandCollection();
	}

	this->cells[this->size] = new (std::nothrow) Cell(cell);
	if (!this->cells[this->size]) {
		std::cerr << "Memory allocation error while adding new cell! Try again!" << std::endl;
		return false;
	}

	//change the width
	if (cell.getValue()) {
		if (this->width < strlen(cell.getValue())) {
			this->width = strlen(cell.getValue());
		}
	}

	this->size += 1;
	return true;
}

const Cell* Column::getCellAt(size_t index) {
	if (index < 0 || index >= this->size) {
		std::cout << "Invalid index!\n";
		std::cout << "For this column valid index would be in range [0, " << this->size - 1 << "]" << std::endl;
		return nullptr;
	}
	return this->cells[index];
}

void Column::changeCellValue(size_t index, const char* newValue) {
	if (index < 0 || index >= this->size) {
		std::cout << "Invalid index!\n";
		return;
	}

	this->cells[index]->setValue(newValue);
}

void Column::removeCellAt(size_t index) {
	if (index < 0 || index >= this->size) {
		std::cout << "Invalid index!\n";
		std::cout << "For this column valid index would be in range [0, " << this->size - 1 << "]" << std::endl;
		return;
	}

	//when we delete a cell, we move the rest of the cells one space backwards and nullptr the last cell
	delete this->cells[index];
	for (size_t i = index; i < this->size - 1; i++) {
		this->cells[i] = this->cells[i + 1]; //to be tested
	}

	this->cells[this->size - 1] = nullptr;

	this->size -= 1;

	//change the width
	this->width = 0;
	for (size_t i = 0; i < this->size; i++) {
		if (!this->cells[i]) {
			continue;
		}
		if (strlen(this->cells[i]->getValue()) > this->width) {
			this->width = strlen(this->cells[i]->getValue());
		}
	}
}

bool Column::swapTwoCells(size_t first, size_t second) {
	if (first < 0 || second < 0 || first >= this->size || second >= this->size) {
		return false;
	}

	Cell* temp = this->cells[first];
	this->cells[first] = this->cells[second];
	this->cells[second] = temp;
	return true;
}

//private methods for internal work
void Column::expandCollection() {
	Cell** tempCollection = new (std::nothrow) Cell*[this->size + 1 + BONUS_CAPACITY];
	if (!tempCollection) {
		throw std::bad_alloc();
	}

	//copying the cells pointers
	for (size_t i = 0; i < this->size; i++) {
		tempCollection[i] = this->cells[i];
	}

	//nullptr-ing the rest of the allocated memory
	for (size_t i = this->size; i < this->size + 1 + BONUS_CAPACITY; i++) {
		tempCollection[i] = nullptr;
	}

	delete[] this->cells;
	this->cells = tempCollection;
	this->allocatedCapacity = this->size + 1 + BONUS_CAPACITY;

	//std::cout << "Collection expanded\n";
}
