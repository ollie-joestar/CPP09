#include <iostream>
#include <fstream>
#include "BitcoinExchange.hpp"

#define RED "\033[31m"
#define RESET "\033[0m"
#define YEL "\033[33m"

int	main(int ac, char **av)
{

	if (ac != 2) {
		std::cout << RED << "Error! Use : ./btc <input_file>" << RESET << std::endl;
		return (1);
	}

	try {
		BitcoinExchange::loadData("data.csv");
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	try {
		BitcoinExchange::processInput(av[1]);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return (0);
}
