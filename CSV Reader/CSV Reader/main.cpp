#include <iostream>
#include "cell.hpp"

int main() {
	Cell a;
	Cell b("abc");
	char word[10];
	std::cin >> word;
	Cell c(word);
	Cell d(c);

	//std::cout << "a: " << a.getValue() << "\n";
	std::cout << "b: " << b.getValue() << "\n";
	std::cout << "c: " << c.getValue() << "\n";
	std::cout << "d: " << d.getValue() << "\n";

	std::cout << "a = b and b = c and d = a\n";
	a = b;
	b = c;
	d = a;

	std::cout << "a: " << a.getValue() << "\n";
	std::cout << "b: " << b.getValue() << "\n";
	std::cout << "c: " << c.getValue() << "\n";
	std::cout << "d: " << d.getValue() << "\n";

	return 0;
}