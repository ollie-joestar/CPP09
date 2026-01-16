#include "PmergeMe.hpp"

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <numbers>" << std::endl;
		return 1;
	}
	std::string input;
    for (int i = 1; i < argc; ++i)
    {
        input += argv[i];
        if (i + 1 < argc)
            input += " ";
    }

	PmergeMe pmergeMe;
	try {
		pmergeMe.sort(input);
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
