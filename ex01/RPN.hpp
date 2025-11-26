#ifndef RPN_HPP
#define RPN_HPP

#include <string>
#include <deque>

class RPN {
public:
	static double evaluate(const std::string &rawProblem);
private:
	static std::deque<double> stack;
	RPN();
	RPN(const RPN& other);
	RPN& operator=(const RPN& other);
	~RPN();

	static double performOperation(char op);
	static bool isOperator(char c);
	static void pushNumber(double number);
	static double popNumber();
};

#endif // RPN_HPP
