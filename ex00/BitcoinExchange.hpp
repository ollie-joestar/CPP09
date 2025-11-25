#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <exception>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sstream>

class BitcoinExchange {
	private:
		static std::map<std::string, double> data;
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange& other);
		BitcoinExchange& operator=(const BitcoinExchange& other);
		~BitcoinExchange();
	public:
		static void loadData(const std::string& filename);
		static double getExchangeRate(const std::string& date);
		static void processInput(const std::string& filename);
		static void processLine(const std::string& line);
		static void printData();
	
		class FileOpenException : public std::exception {
		public:
			virtual const char* what() const throw() {
				return "\033[33mError: Could not open file.\033[0m";
			}
		};
		
		class InvalidData : public std::exception {
		public:
			virtual const char* what() const throw() {
				return "\033[33mError: Invalid data.\033[0m";
			}
		};
};

#endif // BITCOINEXCHANGE_HPP
