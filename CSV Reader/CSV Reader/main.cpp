#include <iostream>
#include "cell.hpp"
#include "utils.hpp"
#include "column.hpp"

int main() {
	//Cell a;
	//Cell b("abc");
	//char word[10];
	//std::cin >> word;
	//Cell c(word);
	//Cell d(c);

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
	Column col(0, ColumnType::Text, "Col");
	std::cout << utilGnrl::columnTypeToStr(col.getType()) << "\n";
	col.setType();
	std::cout << utilGnrl::columnTypeToStr(col.getType()) << std::endl;

	Column e(4, ColumnType::Currency, "HSkeh");
	e = col;
	std::cout << "E info\n";
	std::cout << utilGnrl::columnTypeToStr(e.getType()) << "\n";
	std::cout << e.getName() << " " << e.getSize();
	return 0;
}