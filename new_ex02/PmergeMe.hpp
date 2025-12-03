#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <string>
#include <sstream>
#include <utility>

class PmergeMe {
public:
	PmergeMe();
	~PmergeMe();

	void sort(std::string &input);
private:
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);

	std::vector<std::pair<std::vector<int>, std::vector<int> > > _vec;
	std::vector<std::vector<int> > _vecExtra;

	size_t	initSize;
	size_t	vecComparisonCount;
	size_t	dequeComparisonCount;
	size_t	sorted;
	size_t	toInsert;
	bool 	merged;


	void addNumber(int number);
	void initVectorPairs();
	void sortUsingVector();
	void sortUsingDeque();
	void printVector() const;
	void printDeque() const;

	void compareVector();
	void mergeVector();
	void splitVector();
	void insertVector();
};

#endif // PMERGEME_HPP
