#include <iostream>
#include "cell.hpp"
#include "utils.hpp"
#include "column.hpp"

bool istValidEGN(const char* value) {
	if (strlen(value) != 10) return false;

	int year = (value[0] - '0') * 10 + (value[1] - '0');
	int month = (value[2] - '0') * 10 + (value[3] - '0');
	int day = (value[4] - '0') * 10 + (value[5] - '0');

	//date check
	if (month >= 1 && month <= 12) {
		year += 1900;
	}
	else if (month >= 21 && month <= 32) {
		year += 1800;
		month -= 20;
	}
	else if (month >= 41 && month <= 52) {
		year += 2000;
		month -= 40;
	}
	else {
		return false;
	}

	if (day < 1 || day > 31 || month < 1 || month > 12) return false;

	//controll number check
	int coeficients[9] = { 2, 4, 8, 5, 10, 9, 7, 3, 6 };
	int sum = 0;
	for (int i = 0; i < 9; i++) {
		sum += (value[i] - '0') * coeficients[i];
	}

	int remainder = sum % 11;
	int checkDigit = remainder < 10 ? remainder : 0;

	if (checkDigit == (value[9] - '0')) {
		return true;
	}
	else {
		return false;
	}
}

int main() {
	Cell a;
	Cell b("b");
	//char word[10];
	//std::cin >> word;
	Cell c("c");
	Cell d("d");

	////std::cout << "a: " << a.getValue() << "\n";
	//std::cout << "b: " << b.getValue() << "\n";
	//std::cout << "c: " << c.getValue() << "\n";
	//std::cout << "d: " << d.getValue() << "\n";

	//std::cout << "a = b and b = c and d = a\n";
	//a = b;
	//b = c;
	//d = a;

	//std::cout << "a: " << a.getValue() << "\n";
	//std::cout << "b: " << b.getValue() << "\n";
	//std::cout << "c: " << c.getValue() << "\n";
	//std::cout << "d: " << d.getValue() << "\n";
	/*Column col(0, ColumnType::Text, "Col");
	std::cout << utilGnrl::columnTypeToStr(col.getType()) << "\n";
	col.setType();
	std::cout << utilGnrl::columnTypeToStr(col.getType()) << std::endl;

	Column e(4, ColumnType::Currency, "HSkeh");
	e = col;
	std::cout << "E info\n";
	std::cout << utilGnrl::columnTypeToStr(e.getType()) << "\n";
	std::cout << e.getName() << " " << e.getSize();*/

	Column t(ColumnType::Text, "t");
	t.addCell(a);
	t.addCell(b);
	t.addCell(c);
	t.addCell(d);

	//for (int i = 0; i < 13; i++) {
	//	std::cout << i << ": ";
	//	char word[10];
	//	std::cin >> word;
	//	Cell temp(word);
	//	t.insertCellAt(3, temp);
	//}

	/*t.removeCellAt(2);

	for (int i = 0; i < t.getSize(); i++) {
		if (t[i]->getValue()) {
			std::cout << i << ": " << t[i]->getValue() << "\n";
		}
		else {
			std::cout << i << ": NULL\n";
		}
	}*/

	return 0;
}