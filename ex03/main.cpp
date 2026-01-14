#include "PmergeMe.hpp"

int main(int argc, char** argv) {
	// if (argc != 2) {
	// 	std::cerr << "Usage: " << argv[0] << " <numbers>" << std::endl;
	// 	return 1;
	// }
	size_t test;
	test = 2 / 2;
	std::cout << "Test: " << test << std::endl;
	std::string input;
	if (argc == 2) 
		input = argv[1];
	else
		// input = "20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0";
		input = "0 20 1 19 2 18 3 17 4 16 5 15 6 14 7 13 8 12 9 11 10";
		// input = "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20";

	PmergeMe pmergeMe;
	pmergeMe.sort(input);

	return 0;
}
