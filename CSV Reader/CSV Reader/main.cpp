#include <iostream>
#include "cell.hpp"
#include "utils.hpp"
#include "column.hpp"

int main() {
	Cell a;
	Cell b("b");
	char word[10];
	std::cin >> word;
	Cell c(word);
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
	t.insertCellAt(1, c);
	t.insertCellAt(1, d);
	int index = 0;

	for (int i = 0; i < 4; i++) {
		if (!t.getCellAt(i)->getValue()) {
			std::cout << "Ampty cell\n";
		}
		else {
			std::cout << t.getCellAt(i)->getValue() << "\n";
		}
	}
	return 0;
}