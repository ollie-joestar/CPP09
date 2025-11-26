#include "PmergeMe.hpp"

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <numbers>" << std::endl;
		return 1;
	}

	std::string input = argv[1];
	PmergeMe pmergeMe;
	pmergeMe.sort(input);

	return 0;
}
