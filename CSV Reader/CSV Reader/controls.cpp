#include "controls.hpp"
#include "iostream"
#include "table.hpp"

#define MAX_FILE_NAME 128

Table table;

void clearInputBuffer() {
	char ch;
	while ((ch = std::cin.get()) != '\n' && ch != EOF) {
		// discard leftover characters
	}
}

bool openExistingFile() {
	char fileName[MAX_FILE_NAME];
	std::cout << "Enter file name: ";
	clearInputBuffer();
	std::cin.getline(fileName, MAX_FILE_NAME);
	bool isOpened = table.populateTable(fileName);
	std::cout << "\n";
	return isOpened;
}

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
			<< "7 - Exit program"
			<< std::endl;

		std::cout << "Option: ";
		std::cin >> option;

		if (option < 1 || option > 7) {
			std::cout << "Invalid option!\n";
			continue;
		}

		if (option == 1) {

		}

	} while (true);
}

void controls::run() {
	//initial start of the program
	int option = 0;
	do {
		std::cout 
			<< "1 - Open existing file\n"
			<< "2 - Create new file\n"
			<< "3 - Exit"
			<< std::endl;
		std::cout << "Option: ";
		std::cin >> option;

		if (option < 1 || option > 3) {
			std::cout << "Invalid option!\n";
			continue;
		}

		//existing file options
		if (option == 1) {
			bool isOpened = openExistingFile();
			table.printTable();
		}
		else if (option == 2) {

		}
		else if (option == 3) {
			return;
		}
		else {

		}
	} 
	while (option != 4);
}
