#include "test.h"

#include <iostream>

int main() {

	std::cout << "Calling a test function from engine" << std::endl;
	test();
	std::cout << "Finished calling a test function from engine" << std::endl;
	int a;
	std::cin >> a;
}