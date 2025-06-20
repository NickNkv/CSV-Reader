#include "controls.hpp"
#include "iostream"
#include <fstream>
#include "table.hpp"
#include "column.hpp"
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
	char orderOption[6];

	std::cout << "Sort by column. Enter column name/number: ";
	clearInputBuffer();
	std::cin.getline(colOption, MAX_FILE_NAME);

	std::cout << "Ascending or descending (asc/desc): ";
	std::cin.getline(orderOption, 6);

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
		return (*table).sort((*table).findColByName(colOption), ascending);
	}
}

void filterTable() {
	char colName[MAX_FILE_NAME];
	char condition[4];
	char value[MAX_FILE_NAME];

	std::cout << "Choose column by wich to filter (name/number): ";
	clearInputBuffer();
	std::cin.getline(colName, MAX_FILE_NAME);

	std::cout << "Enter condition (==, !=, <, <=, >, >=): ";
	std::cin.getline(condition, 4);

	std::cout << "Enter condition value: ";
	std::cin.getline(value, MAX_FILE_NAME);

	//column can be found via name or order
	if (util::isNum(colName)) {
		(*table).filter(atoi(colName) - 1, condition, value);
	}
	else {
		(*table).filter((*table).findColByName(colName), condition, value);
	}
}

bool addColumn() {
	try {
		Column col;
		char name[MAX_FILE_NAME];
		std::cout << "Enter the column name: ";
		clearInputBuffer();
		std::cin.getline(name, MAX_FILE_NAME);
		col.setName(name);

		col.setType();

		std::cout << "Add data:" << std::endl;
		clearInputBuffer();
		for (size_t i = 0; i < (*table).getRowCount(); i++) {
			std::cout << i + 1 << ": ";
			std::cin.getline(name, MAX_FILE_NAME);

			Cell temp(name);
			col.addCell(temp);
		}

		return (*table).addColumn(col);
	}
	catch (std::exception& e) {
		std::cout << "Error while adding new column!\n";
		std::cout << e.what();
		return false;
	}
}

bool deleteCol() {
	size_t index = 0;
	std::cout << "Which column do you want to delete (0 for exit): ";
	std::cin >> index;

	if (index == 0) {
		return false;
	}
	else {
		(*table).removeColumnAt(index - 1);
		return true;
	}
}

bool changeColName() {
	char newName[MAX_FILE_NAME];
	char colName[MAX_FILE_NAME];

	std::cout << "Choose the column for name-change (name/number): ";
	clearInputBuffer();
	std::cin.getline(colName, MAX_FILE_NAME);

	std::cout << "Enter new name: ";
	std::cin.getline(newName, MAX_FILE_NAME);


	//column can be found via name or order
	if (util::isNum(colName)) {
		return (*table).changeColumnName(atoi(colName) - 1, newName);
	}
	else {
		return (*table).changeColumnName((*table).findColByName(colName), newName);
	}
}

bool saveChanges() {
	int option = 0;
	do {
		std::cout
				<< "1 - Save to current file\n"
				<< "2 - Save to another/new file"
				<< std::endl;

		std::cout << "Option: ";
		std::cin >> option;

		if (option == 1) {
			return (*table).saveToFile(fileName);
		}
		else if (option == 2) {
			char newFile[MAX_FILE_NAME];
			std::cout << "Enter the destination file name: ";
			clearInputBuffer();
			std::cin.getline(newFile, MAX_FILE_NAME);
			return (*table).saveToFile(newFile);
		}
		else {
			std::cout << "Invalid option!\n";
			continue;
		}
	} while (true);
}

//settings panel
void settingsMenu() {
	int option = 0;
	do
	{
		std::cout
			<< "1 - Change delimiter\n"
			<< "2 - Exit\n"
			<< std::endl;

		std::cout << "Option: ";
		std::cin >> option;

		if (option == 1) {
			char newDeliimiter[6];
			std::cout << "Enter new delimiter (up to 5 symbols): ";
			clearInputBuffer();
			std::cin.getline(newDeliimiter, 6);
			(*table).setDelimiter(newDeliimiter);
		}
		else if (option == 2) {
			return;
		}
		else {
			std::cout << "\nInvalid option, try again!\n";
		}
	} while (true);
}

//menu when we have populated table
void tableManipulationMenu() {
	int option = 0;
	bool isChanged = false;
	do {
		std::cout 
			<< "1 - Sort\n"
			<< "2 - Filter\n"
			<< "3 - Remove identical rows\n"
			<< "4 - Add column\n"
			<< "5 - Delete column\n"
			<< "6 - Change column name\n"
			<< "7 - Save changes\n"
			<< "8 - Undo\n"
			<< "9 - Add-to-end-of-table options\n"
			<< "9 - Settings\n"
			<< "10 - Exit"
			<< std::endl;

		std::cout << "Option: ";
		std::cin >> option;

		bool flag = false;
		if (option == 1) {
			flag = sort();

			if (flag) isChanged = true;
			else std::cout << "Sort returned error, please try again!" << std::endl;

			(*table).printTable();
		}
		else if (option == 2) {
			filterTable();
			(*table).printTable();
			isChanged = true;
		}
		else if (option == 3) {
			(*table).removeIdenticalRows();
			(*table).printTable();
			isChanged = true;
		}
		else if (option == 4) {
			flag = addColumn();
			if (flag) {
				isChanged = true;
				std::cout << "\nColumn added!\n\n";
			}
			(*table).printTable();
		}
		else if (option == 5) {
			flag = deleteCol();
			if (flag) {
				isChanged = true;
			}

			(*table).printTable();
		}
		else if (option == 6) {
			flag = changeColName();
			if (flag) {
				isChanged = true;
			}
			else {
				std::cout << "Unable to change name, try again!\n\n";
			}
			(*table).printTable();
		}
		else if (option == 7) {
			flag = saveChanges();

			if (flag) {
				isChanged = false;
				std::cout << "Changes saved!" << std::endl;
			}
			else std::cout << "Save changes returned error, please try again!" << std::endl;
			(*table).printTable();
		} 
		else if (option == 8) {
		}
		else if (option == 9) {
			settingsMenu();
			(*table).printTable();
		}
		else if (option == 10) { 
			//exit
			if (isChanged) {
				std::cout << "\nThere are unsaved changes!\nDo you want to save them (yes/no): ";
				char answer[5];
				clearInputBuffer();
				std::cin.getline(answer, 4);
				if (strcmp(answer, "yes") == 0) {
					flag = saveChanges();

					if (flag) {
						isChanged = false;
						std::cout << "Changes saved!\n\n";
					}
					else {
						std::cout << "Save changes returned error, please try again!" << std::endl;
						continue;
					}
				}
			}
			std::cout << "\n";
			(*table).emptyTable();
			return;
		} 
		else {
			std::cout << "Invalid option!\n";
		}

	} while (true);
}

//initial start of the program
void controls::run() {
	//create new empty table if there isnt one already
	if (!isLoaded) {
		try {
			table = new Table();
			isLoaded = true;
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
			continue;
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
