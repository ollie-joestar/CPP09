#include "RPN.hpp"
#include <iostream>
#include <climits>

std::deque<double> RPN::stack;

RPN::RPN() {}

RPN::~RPN() {}

bool RPN::isOperator(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/';
}

void RPN::pushNumber(double number) {
	stack.push_back(number);
}

double RPN::popNumber() {
	if (stack.empty()) {
		std::cerr << "\033[31mError: Skill issue. Not enough numbers\033[0m" << std::endl;
		return (-1);
	}
	double number = stack.back();
	stack.pop_back();
	return number;
}

double RPN::performOperation(char op) {
	double right = popNumber();
	double left = popNumber();
	if (right == -1 || left == -1)
		return (-1);

	double result;
	long check;
	switch (op) {
		case '+':
			check = static_cast<long>(left) + static_cast<long>(right);
			if (check > INT_MAX || check < INT_MIN) {
				std::cerr << "\033[31mError: Skill Issue. Integer overflow on addition.\033[0m" << std::endl;
				return (-1);
			}
			result = left + right;
			break;
		case '-':
			check = static_cast<long>(left) - static_cast<long>(right);
			if (check > INT_MAX || check < INT_MIN) {
				std::cerr << "\033[31mError: Skill Issue. Integer overflow on subtraction.\033[0m" << std::endl;
				return (-1);
			}
			result = left - right;
			break;
		case '*':
			check = static_cast<long>(left) * static_cast<long>(right);
			if (check > INT_MAX || check < INT_MIN) {
				std::cerr << "\033[31mError: Skill Issue. Integer overflow on multiplication.\033[0m" << std::endl;
				return (-1);
			}
			result = left * right;
			break;
		case '/':
			if (right == 0) {
				std::cerr << "\033[31mError: Skill Issue. Division by zero.\033[0m" << std::endl;
				return (-1);
			}
			check = static_cast<long>(left) / static_cast<long>(right);
			if (check > INT_MAX || check < INT_MIN) {
				std::cerr << "\033[31mError: Skill Issue. Integer overflow on division.\033[0m" << std::endl;
				return (-1);
			}
			result = left / right;
			break;
		default:
			std::cerr << "\033[31mError: Skill Issue. Unknown operator '" << op << "'.\033[0m" << std::endl;
			return (-1);
	}
	pushNumber(result);
	return (0);
}

static bool isValidCharacter(char c) {
	return (c >= '0' && c <= '9') || c == '+' || c == '-' || c == '*' || c == '/';
}

static bool isSpace(char c) {
	return c == ' ' || c == '\t' || c == '\n';
}

static std::string cleanInput(const std::string &input) {
	std::string cleaned;
	bool inSpace = false;
	for (size_t i = 0; i < input.length(); ++i) {
		char c = input[i];
		if (isSpace(c) && i != 0) {
			if (!inSpace) {
				cleaned += ' ';
				inSpace = true;
			}
		} else if (isValidCharacter(c)) {
			cleaned += c;
			inSpace = false;
		} else {
			std::cerr << "\033[31mError: Skill Issue. Invalid character '" << c << "' in input.\033[0m" << std::endl;
			return "";
		}
	}
	return cleaned;
}

double RPN::evaluate(const std::string &rawProblem) {
	if (rawProblem.empty()) {
		std::cerr << "\033[31mError: Skill Issue. Empty input.\033[0m" << std::endl;
		return (-1);
	}
	std::string problem = cleanInput(rawProblem);
	if (problem.empty()) {
		return (-1);
	}
	for (size_t i = 0; i < problem.length(); ++i) {
		if (!isValidCharacter(problem[i]) && !isSpace(problem[i])) {
			std::cerr << "\033[31mError: Skill Issue. Invalid character '" << problem[i] << "' in input.\033[0m" << std::endl;
			return (-1);
		}
	}
	for (size_t i = 0; i < problem.length(); ++i) {
		if (isSpace(problem[i])) {
	  		continue;
		}
		char c = problem[i];
		if (c >= '0' && c <= '9') {
			pushNumber(c - '0');
		} else if (isOperator(c)) {
			if (-1 == performOperation(c))
				return (-1);
		} else {
			std::cerr << "\033[31mError: Skill Issue. Invalid character '" << c << "' in input.\033[0m" << std::endl;
			return (-1);
		}
	}
	if (stack.size() != 1) {
		std::cerr << "\033[31mError: Skill Issue. The RPN expression is invalid.\033[0m" << std::endl;
		return (-1);
	}
	double result = popNumber();
	std::cout << std::fixed << result << std::endl;
	return result;
}
