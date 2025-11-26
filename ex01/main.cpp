#include "RPN.hpp"
#include <iostream>

int	main(int ac, char **av) {
	if (ac != 2) {
		std::cerr << "\033[31mError: Skill Issue. Invalid number of arguments.\033[0m" << std::endl;
		return (1);
	}
	RPN::evaluate(av[1]);
	return (0);
}
