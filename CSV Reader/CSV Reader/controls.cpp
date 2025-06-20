#define _CRT_SECURE_NO_WARNINGS
#include "controls.hpp"
#include "iostream"
#include <fstream>
#include "table.hpp"
#include "column.hpp"
#include "utils.hpp"

#define MAX_FILE_NAME 128
#define MAX_COL 100

Table* table = nullptr;
bool isLoaded = false;

int visualizationOption = 0;

Table* tableSnapshot = nullptr;

char fileName[MAX_FILE_NAME];

bool openExistingFile() {
	//char fileName[MAX_FILE_NAME];
	std::cout << "Enter file name: ";
	util::clearInputBuffer();
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
	util::clearInputBuffer();
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
		std::cout << "Invalid sort option!\n";
		return false;
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
	util::clearInputBuffer();
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
		util::clearInputBuffer();
		std::cin.getline(name, MAX_FILE_NAME);
		col.setName(name);

		col.setType();

		std::cout << "Add data:" << std::endl;
		util::clearInputBuffer();
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
	char option[MAX_FILE_NAME];
	std::cout << "Which column do you want to delete ('no' to cancel): ";
	util::clearInputBuffer();
	std::cin.getline(option, MAX_FILE_NAME);

	if (strcmp(option, "no") == 0) {
		return false;
	}
	else if (util::isNum(option)) {
		(*table).removeColumnAt(atoi(option) - 1);
		return true;
	}
	else {
		(*table).removeColumnAt((*table).findColByName(option));
		return true;
	}
}

bool duplicateCol() {
	char colName[MAX_FILE_NAME];

	std::cout << "Choose a column to duplicate (name/number): ";
	util::clearInputBuffer();
	std::cin.getline(colName, MAX_FILE_NAME);

	//column can be found via name or order
	if (util::isNum(colName)) {
		return (*table).duplicateColumn(atoi(colName) - 1);
	}
	else {
		return (*table).duplicateColumn((*table).findColByName(colName));
	}
}

bool changeColOrder() {
	char input[MAX_FILE_NAME];
	std::cout << "Enter the new col orde (ex. 2 1 3): ";
	util::clearInputBuffer();
	std::cin.getline(input, MAX_FILE_NAME);

	unsigned count = 0;
	unsigned* result = new (std::nothrow) unsigned[MAX_COL];
	if (!result) {
		std::cout << "Memory allocation error! Try again!\n\n";
		return false;
	}

	char* token = strtok(input, " ");
	while (token != nullptr) {
		if (!util::isNum(token)) {
			delete[] result;
			std::cout << "Invalid number input!\n\n";
			return false;
		}

		result[count++] = atoi(token);
		token = strtok(nullptr, " ");
	}

	bool flag = (*table).changeColumnOrder(result, count);
	delete[] result;
	return flag;
}

bool changeColName() {
	char newName[MAX_FILE_NAME];
	char colName[MAX_FILE_NAME];

	std::cout << "Choose the column for name-change (name/number): ";
	util::clearInputBuffer();
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
			util::clearInputBuffer();
			std::cin.getline(newFile, MAX_FILE_NAME);
			return (*table).saveToFile(newFile);
		}
		else {
			std::cout << "Invalid option!\n";
			continue;
		}
	} while (true);
}

void copyRow() {
	char option[10];
	std::cout << "Enter row number (0 for exit): ";
	util::clearInputBuffer();
	std::cin.getline(option, 10);
	if (util::isNum(option)) {
		if (atoi(option) == 0) {
			return;
		}
		else {
			(*table).copyRow(atoi(option) - 1);
		}
	}
	else {
		std::cout << "Invalid input!\n";
	}
}

void saveUndoSnapshot() {
	(*tableSnapshot) = (*table);
}

void undoChanges() {
	char option[MAX_FILE_NAME];
	std::cout << "Do you want to undo changes (yes/no): ";
	util::clearInputBuffer();
	std::cin.getline(option, MAX_FILE_NAME);

	if (strcmp(option, "yes") == 0) {
		(*table) = (*tableSnapshot);
	}
	else {
		return;
	}
}

void endOfTableOptionsMenu(bool& isChanged) {
	std::cout << "\n";
	(*table).printTable(visualizationOption);
	int option = 0;
	do {
		std::cout
			<< "Add row to the end of the table\n"
			<< "1 - Copy row\n"
			<< "2 - Min values\n"
			<< "3 - Max values\n"
			<< "4 - Most frequent\n"
			<< "5 - Exit\n"
			<< std::endl;

		std::cout << "Option: ";
		std::cin >> option;

		if (option == 1) {
			saveUndoSnapshot();
			copyRow();
			isChanged = true;
			(*table).printTable(visualizationOption);
		}
		else if (option == 2) {
			saveUndoSnapshot();
			(*table).addExtremeValues(true);
			isChanged = true;
			(*table).printTable(visualizationOption);
		}
		else if (option == 3) {
			saveUndoSnapshot();
			(*table).addExtremeValues(false);
			isChanged = true;
			(*table).printTable(visualizationOption);
		}
		else if (option == 4) {

		}
		else if (option == 5) {
			return;
		}
		else {
			std::cout << "\nInvalid option, try again!\n";
		}
	} while (true);
}

bool editCell() {
	char colName[MAX_FILE_NAME];
	char colNum[10];
	char newData[MAX_FILE_NAME];

	std::cout << "Choose the column (name/number): ";
	util::clearInputBuffer();
	std::cin.getline(colName, MAX_FILE_NAME);

	std::cout << "Choose row (number): ";
	std::cin.getline(colNum, 10);

	std::cout << "Enter new value: ";
	std::cin.getline(newData, MAX_FILE_NAME);

	if (!util::isNum(colNum)) {
		std::cout << "Invalid input!\n";
		return false;
	}

	if (util::isNum(colName)) {
		return (*table).editCell(atoi(colName) - 1, atoi(colNum) - 1, newData);
	}
	else {
		return (*table).editCell((*table).findColByName(colName), atoi(colNum) - 1, newData);
	}
}

void visualizationMenu() {
	int option = 0;
	std::cout
		<< "Choose style:\n"
		<< "1 - Default\n"
		<< "2 - Fancy"
		<< std::endl;

	std::cout << "Option: ";
	std::cin >> option;
	std::cout << "\n";

	if (option == 1) {
		visualizationOption = 0;
	}
	else if (option == 2) {
		visualizationOption = 1;
	}
	else {
		visualizationOption = 0;
	}
}

//settings panel
void settingsMenu() {
	int option = 0;
	do
	{
		std::cout
			<< "1 - Change delimiter\n"
			<< "2 - Change visualization\n"
			<< "3 - Exit\n"
			<< std::endl;

		std::cout << "Option: ";
		std::cin >> option;

		if (option == 1) {
			char newDeliimiter[6];
			std::cout << "Enter new delimiter (up to 5 symbols): ";
			util::clearInputBuffer();
			std::cin.getline(newDeliimiter, 6);
			(*table).setDelimiter(newDeliimiter);
		}
		else if (option == 2) {
			visualizationMenu();
		}
		else if (option == 3) {
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
			<< "6 - DuplicateColumn\n"
			<< "7 - Change column order\n"
			<< "8 - Change column name\n"
			<< "9 - Save changes\n"
			<< "10 - Undo\n"
			<< "11 - Add-to-end-of-table options\n"
			<< "12 - Edit cell\n"
			<< "13 - Add row\n"
			<< "14 - Settings\n"
			<< "15 - Exit"
			<< std::endl;

		std::cout << "Option: ";
		std::cin >> option;

		bool flag = false;
		if (option == 1) {
			saveUndoSnapshot();
			flag = sort();

			if (flag) {
				isChanged = true;
			}
			else std::cout << "Sort returned error, please try again!" << std::endl;

			(*table).printTable(visualizationOption);
		}
		else if (option == 2) {
			saveUndoSnapshot();
			filterTable();
			(*table).printTable(visualizationOption);
			isChanged = true;
		}
		else if (option == 3) {
			saveUndoSnapshot();
			(*table).removeIdenticalRows();
			(*table).printTable(visualizationOption);
			isChanged = true;
		}
		else if (option == 4) {
			saveUndoSnapshot();
			flag = addColumn();
			if (flag) {
				isChanged = true;
				std::cout << "\nColumn added!\n\n";
			}
			(*table).printTable(visualizationOption);
		}
		else if (option == 5) {
			saveUndoSnapshot();
			flag = deleteCol();
			if (flag) {
				isChanged = true;
			}

			(*table).printTable(visualizationOption);
		}
		else if (option == 6) {
			saveUndoSnapshot();
			flag = duplicateCol();
			if (flag) {
				isChanged = true;
			}

			std::cout << "\n";
			(*table).printTable(visualizationOption);
		}
		else if (option == 7) {
			saveUndoSnapshot();
			flag = changeColOrder();
			if (flag) {
				isChanged = true;
			}
			(*table).printTable(visualizationOption);
		}
		else if (option == 8) {
			saveUndoSnapshot();
			flag = changeColName();
			if (flag) {
				isChanged = true;
			}
			else {
				std::cout << "Unable to change name, try again!\n\n";
			}
			(*table).printTable(visualizationOption);
		}
		else if (option == 9) {
			flag = saveChanges();

			if (flag) {
				isChanged = false;
				std::cout << "Changes saved!" << std::endl;
			}
			else std::cout << "Save changes returned error, please try again!" << std::endl;
			(*table).printTable(visualizationOption);
		} 
		else if (option == 10) {
			undoChanges();
			(*table).printTable(visualizationOption);
		}
		else if (option == 11) {
			endOfTableOptionsMenu(isChanged);
			(*table).printTable(visualizationOption);
		}
		else if (option == 12) {
			saveUndoSnapshot();
			isChanged = editCell();
			(*table).printTable(visualizationOption);
		}
		else if (option == 13) {
			saveUndoSnapshot();
			try {
				isChanged = (*table).addRow();
			}
			catch (std::exception& e) {
				std::cout << e.what() << std::endl;
			}
			(*table).printTable(visualizationOption);
		}
		else if (option == 14) {
			settingsMenu();
			(*table).printTable(visualizationOption);
		}
		else if (option == 15) { 
			//exit
			if (isChanged) {
				std::cout << "\nThere are unsaved changes!\nDo you want to save them (yes/no): ";
				char answer[5];
				util::clearInputBuffer();
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

void newFile() {
	std::cout << "Enter file name: ";
	util::clearInputBuffer();
	std::cin.getline(fileName, MAX_FILE_NAME);

	addColumn();
	saveChanges();
	(*table).printTable(visualizationOption);
	saveUndoSnapshot();
	tableManipulationMenu();
}

//initial start of the program
void controls::run() {
	//create new empty table if there isnt one already
	if (!isLoaded) {
		try {
			table = new Table("t1"); //name could be changed
			isLoaded = true;
			tableSnapshot = new Table("t1");
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
				(*table).printTable(visualizationOption);
				saveUndoSnapshot();
				tableManipulationMenu();
			}
			continue;
		}
		else if (option == 2) {
			newFile();
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
