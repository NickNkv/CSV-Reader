#include "controls.hpp"
#include "iostream"
#include <fstream>
#include "table.hpp"
#include "utils.hpp"

#define MAX_FILE_NAME 128

Table* table = nullptr;
bool isLoaded = false;
char fileName[MAX_FILE_NAME];

void clearInputBuffer() {
	char ch;
	while ((ch = std::cin.get()) != '\n' && ch != EOF) {
		// discard leftover characters
	}
}

bool openExistingFile() {
	//char fileName[MAX_FILE_NAME];
	std::cout << "Enter file name: ";
	clearInputBuffer();
	std::cin.getline(fileName, MAX_FILE_NAME);
	bool isOpened = (*table).populateTable(fileName);
	std::cout << "\n";
	return isOpened;
}

//sorting by column
bool sort() {
	char colOption[MAX_FILE_NAME];
	char orderOption[MAX_FILE_NAME];

	std::cout << "Sort by column. Enter column name/number: ";
	clearInputBuffer();
	std::cin.getline(colOption, MAX_FILE_NAME);

	std::cout << "Ascending or descending (asc/desc): ";
	clearInputBuffer();
	std::cin.getline(orderOption, MAX_FILE_NAME);

	bool ascending = true;
	if (strcmp(orderOption, "asc") == 0) {
		ascending = true;
	}
	else if (strcmp(orderOption, "desc") == 0) {
		ascending = false;
	}
	else {
		ascending = true;
	}

	//column can be found via name or order
	if (util::isNum(colOption)) {
		return (*table).sort(atoi(colOption) - 1, ascending);
	}
	else {
		return (*table).sortByColName(colOption, ascending);
	}
}

bool saveChanges() {
	std::ofstream out(fileName);
	//if (!out.is_open())
	return true;
}

//menu when we have populated table
void tableManipulationMenu() {
	int option = 0;
	do {
		std::cout 
			<< "1 - Sort\n"
			<< "2 - Filter\n"
			<< "3 - Remove identical rows\n"
			<< "4 - Add column\n"
			<< "5 - Delete column\n"
			<< "6 - Change column name\n"
			<< "7 - Save changes\n"
			<< "8 - Settings\n"
			<< "9 - Exit"
			<< std::endl;

		std::cout << "Option: ";
		std::cin >> option;

		bool flag = false;
		if (option == 1) {
			flag = sort();
			if (!flag) {
				std::cout << "Sort returned error, please try again!" << std::endl;
			}

			(*table).printTable();
		}
		else if (option == 2) {
			
		}
		else if (option == 7) {
			flag = saveChanges();
		}
		else if (option == 9) {
			return;
		}
		else {
			std::cout << "Invalid option!\n";
			continue;
		}

	} while (true);
}

//initial start of the program
void controls::run() {
	//create new empty table if there isnt one already
	if (!isLoaded) {
		try {
			table = new Table();
		}
		catch (const std::exception& e) {
			std::cout << "Error occured in initial run() function!\n";
			std::cout << e.what();
			return;
		}
	}

	int option = 0;
	do {
		std::cout 
			<< "1 - Open existing file\n"
			<< "2 - Create new file\n"
			<< "3 - Exit"
			<< std::endl;
		std::cout << "Option: ";
		std::cin >> option;

		//existing file options
		if (option == 1) {
			bool isOpened = openExistingFile();
			if (isOpened) {
				(*table).printTable();
				tableManipulationMenu();
			}
		}
		else if (option == 2) {

		}
		else if (option == 3) {
			return;
		}
		else {
			std::cout << "Invalid option!\n";
			continue;
		}
	} 
	while (true);
}
