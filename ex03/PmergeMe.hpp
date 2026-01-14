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

	std::vector<std::vector<int> > _vecA;
	std::vector<std::vector<int> > _vecInsert;
	std::vector<std::vector<int> > _vecExtra;
	std::vector<std::vector<int> > _vecSorted;

	size_t	initSize;
	size_t	vecComparisonCount;
	size_t	dequeComparisonCount;
	size_t	sorted;
	size_t	toInsert;
	bool 	merged;

	void addNumber(int number);
	void sortUsingVector();
	void mergeVector();
	void compareVector();
	void splitVector();
	void strangerVecAlert();
	void insertVector();

	size_t findRange(size_t idx) const;
	void printVector() const;
	void printDeque() const;
};

#endif // PMERGEME_HPP
