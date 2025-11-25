#include "BitcoinExchange.hpp"

std::map<std::string, double> BitcoinExchange::data;

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::printData() {
	if (data.empty()) {
		std::cout << "No data loaded." << std::endl;
		return;
	}
	for (std::map<std::string, double>::const_iterator it = data.begin(); it != data.end(); ++it) {
		std::cout << it->first << " => " << it->second << std::endl;
	}
}

static void validateData(const std::string& date, const std::string& value,
						 double rate) { 
	if (date.length() != 10 || date[4] != '-' || date[7] != '-') {
		throw BitcoinExchange::InvalidData();
	}
	if (date.substr(0, 4) < "2009") { // Bitcoin started in 2009
		throw BitcoinExchange::InvalidData();
	}
	if (date.substr(5, 2) < "01" || date.substr(5, 2) > "12") {
		throw BitcoinExchange::InvalidData();
	}
	if (date.substr(8, 2) < "01" || date.substr(8, 2) > "31") {
		throw BitcoinExchange::InvalidData();
	}
	if (date.substr(5, 2) == "02") { // February
		if (std::atoi(date.substr(8, 2).c_str()) >
			28 + (std::atoi(date.substr(0, 4).c_str()) % 4 == 0)) {
			throw BitcoinExchange::InvalidData();
		}
	}

	int	 dot_count = 0;
	for (size_t i = 0; i < value.length(); ++i) {
		if (!isdigit(value[i]) && value[i] != '.') {
			throw BitcoinExchange::InvalidData();
		}
		dot_count += value[i] == '.' ? 1 : 0;
		if (dot_count > 1) {
			throw BitcoinExchange::InvalidData();
		}
	}
	if (rate < 0) {
		throw BitcoinExchange::InvalidData();
	}
}

static double parseRate(const std::string& rate_str) {
	std::stringstream ss(rate_str);
	double value;
	ss >> value;
	if (ss.fail() || !ss.eof()) {
		return -1;
	}

	char *end;
	value = std::strtod(rate_str.c_str(), &end);
	if (*end) {
		return -1;
	}
	return value;
}

void BitcoinExchange::loadData(const std::string& filename) {
	std::ifstream file(filename.c_str());
	if (!file.is_open()) {
		throw FileOpenException();
	}
	std::string line;
	int line_number = 0;
	while (std::getline(file, line)) {
		if (line_number++ == 0) {
			if (line != "date,exchange_rate") {
				file.close();
				throw InvalidData();
			}
			continue; // Skip header line
		}
		size_t commaPos = line.find(',');
		if (commaPos != std::string::npos) {
			std::string date = line.substr(0, commaPos);
			double rate = parseRate(line.substr(commaPos + 1));
			try {
				validateData(date, line.substr(commaPos + 1), rate);
			} catch (const InvalidData& e) {
				file.close();
				throw;
			}
			data[date] = rate;
		}
		else {
			file.close();
			throw InvalidData();
		}
	};
	file.close();
}

double BitcoinExchange::getExchangeRate(const std::string& date) {
	std::map<std::string, double>::const_iterator it = data.find(date);
	if (it != data.end()) {
		return it->second;
	}
	it = data.lower_bound(date);
	if (it == data.begin()) {
		return -1; // No earlier date available
	}
	--it;
	return it->second;
}

void printResult(const std::string& date, const std::string& value_str, double value, double rate) {
	// too long num
	if (value_str.length() > 16) {
		std::cout << "Error: invalid value " << value_str << std::endl;
		return;
	}
	// no rate in base
	if (rate < 0) {
		std::cout << "Error: no exchange rate available for date " << date << std::endl;
		return;
	}
	// bad format
	if (date.length() != 10 || date[4] != '-' || date[7] != '-') {
		std::cout << "Error: invalid date format for " << date << std::endl;
		return;
	}
	// you're too early
	if (date < "2009-01-02") {
		std::cout << "Error: date " << date << " is before Bitcoin existed." << std::endl;
		return;
	}
	// invalid month
	if (date.substr(5, 2) < "01" || date.substr(5, 2) > "12") {
		std::cout << "Error: invalid month in date " << date << std::endl;
		return;
	}
	// invalid day in January, March, May, July, August, October, December
	if (date.substr(5, 2) == "01" || date.substr(5, 2) == "03" ||
		date.substr(5, 2) == "05" || date.substr(5, 2) == "07" ||
		date.substr(5, 2) == "08" || date.substr(5, 2) == "10" ||
		date.substr(5, 2) == "12") {
		if (date.substr(8, 2) > "31") {
			std::cout << "Error: invalid day in date " << date << std::endl;
			return;
		}
	}
	// invalid day in April, June, September, November
	else if (date.substr(5, 2) == "04" || date.substr(5, 2) == "06" ||
			 date.substr(5, 2) == "09" || date.substr(5, 2) == "11") {
		if (date.substr(8, 2) > "30") {
			std::cout << "Error: invalid day in date " << date << std::endl;
			return;
		}
	}
	// invalid day in general
	if (date.substr(8, 2) < "01") {
		std::cout << "Error: invalid day in date " << date << std::endl;
		return;
	}
	// invalid day in February
	if (date.substr(5, 2) == "02") { // February
		if (std::atoi(date.substr(8, 2).c_str()) >
			28 + (std::atoi(date.substr(0, 4).c_str()) % 4 == 0)) {
			std::cout << "Error: invalid day in date " << date << std::endl;
			return;
		}
	}

	// invalid value format
	int	 dot_count = 0;
	for (size_t i = 0; i < value_str.length(); ++i) {
		if (!isdigit(value_str[i]) && value_str[i] != '.' && value_str[i] != '-') {
			if (i == 0 && value_str[i] == '-') {
				std::cout << "Error: not a positive number." << std::endl;
			} else {
				std::cout << "Error: invalid value " << value_str << std::endl;
			}
			return;
		}
		dot_count += value_str[i] == '.' ? 1 : 0;
		if (dot_count > 1) {
			std::cout << "Error: invalid value " << value_str << std::endl;
			return;
		}
	}
	// negative or too large value
	if (value < 0) {
		std::cout << "Error: not a positive number." << std::endl;
		return;
	}
	// too large value
	if (value > 1000) {
		std::cout << "Error: too large a number." << std::endl;
		return;
	}
	// overflow check
	long overflow_check = static_cast<long>(value * rate);
	if (overflow_check > 2147483647) {
		std::cout << "Error: value " << value_str << " causes overflow." << std::endl;
		return;
	}
	// all checks passed, print result
	std::cout << date << " => " << value_str << " = " << (value * rate) << std::endl;
}

void BitcoinExchange::processLine(const std::string& line) {
	if (line.empty()) {
		std::cout << "Error: line is empty." << std::endl;
		return; // Skip header or empty lines
	}
	size_t pipePos = line.find('|');
	if (pipePos != std::string::npos) {
		size_t begin = line.find_first_not_of(" \t");
		size_t end = line.find_last_not_of(" \t", pipePos - 1);
		if (end == std::string::npos || begin == std::string::npos || begin > end) {
			std::cout << "Error: bad input => " << line << std::endl;
			return;
		}
		std::string date = line.substr(begin, end - begin + 1);
		begin = line.find_first_not_of(" \t", pipePos + 1);
		end = line.find_last_not_of(" \t");
		if (begin == std::string::npos || end == std::string::npos ||
			begin > end) {
			std::cout << "Error: bad input => " << line << std::endl;
			return;
		}
		std::string value_str = line.substr(begin, end - begin + 1);
		double value = parseRate(value_str);
		double rate = getExchangeRate(date);
		printResult(date, value_str, value, rate);
	}
	else {
		std::cout << "Error: bad input => " << line << std::endl;
	}
}

void BitcoinExchange::processInput(const std::string& filename) {
	if (data.empty()) {
		throw InvalidData();
	}
	std::ifstream file(filename.c_str());
	if (!file.is_open()) {
		throw FileOpenException();
	}
	std::string line;
	int	line_number = 0;
	while (std::getline(file, line)) {
		if (line_number++ == 0) {
			size_t begin = line.find_first_not_of(" ");
			size_t end = line.find_last_not_of(" ");
			if (begin == std::string::npos || end == std::string::npos) {
				file.close();
				throw InvalidData();
			}
			if (line.substr(begin, end + 1) != "date | value") {
				file.close();
				throw InvalidData();
			}
			continue; // Skip header line
		}
		processLine(line);
	}
	file.close();
}
