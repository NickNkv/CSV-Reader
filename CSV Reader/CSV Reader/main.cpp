#include <iostream>
#include "cell.hpp"
#include "utils.hpp"
#include "column.hpp"

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

	for (int i = 0; i < 13; i++) {
		std::cout << i << ": ";
		char word[10];
		std::cin >> word;
		Cell temp(word);
		t.insertCellAt(3, temp);
	}

	for (int i = 0; i < 17; i++) {
		if (t[i]->getValue()) {
			std::cout << i << ": " << t[i]->getValue() << "\n";
		}
		else {
			std::cout << i << ": NULL\n";
		}
	}
	return 0;
}