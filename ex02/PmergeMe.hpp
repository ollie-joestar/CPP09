#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <string>
#include <sstream>

class PmergeMe {
public:
	PmergeMe();
	~PmergeMe();

	void sort(std::string &input);
private:
	std::vector<std::vector<int> > _numVector;
	std::vector<std::vector<int> > _sortedVector;
	std::deque<std::deque<int> > _numDeque;
	std::deque<std::deque<int> > _sortedDeque;

	size_t	initSize;
	size_t	vecComparisonCount;
	size_t	dequeComparisonCount;
	size_t	sorted;
	size_t	toInsert;

	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);

	void addNumber(int number);
	void sortUsingVector();
	void sortUsingDeque();
	void printVector() const;
	void printDeque() const;

	void mergeSortVector();
	void mergeSortDeque();

	void splitVector();
	void splitDeque();
	void mergeInsertVector();
	void mergeInsertDeque();
};

#endif // PMERGEME_HPP
