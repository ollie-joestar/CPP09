#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <string>
#include <sstream>
#include <utility>
#include <sys/time.h>

class PmergeMe {
public:
	PmergeMe();
	~PmergeMe();

	void sort(std::string &input);
private:
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);

	std::vector<std::vector<int> > _vec;
	std::vector<std::vector<int> > _vecInsert;
	std::vector<std::vector<int> > _vecExtra;
	std::vector<std::vector<int> > _vecSorted;

	std::deque<std::deque<int> > _deq;
	std::deque<std::deque<int> > _deqInsert;
	std::deque<std::deque<int> > _deqExtra;
	std::deque<std::deque<int> > _deqSorted;

	size_t	initSize;
	size_t	vecComparisonCount;
	size_t	dequeComparisonCount;
	size_t	sorted;
	size_t	toInsert;
	bool 	merged;
	long	vecTime;
	long	deqTime;

	void addNumber(int number);
	void sortUsingVector();
	void sortUsingDeque();
	void mergeVector();
	void mergeDeque();
	void compareVector();
	void compareDeque();
	void splitVector();
	void splitDeque();
	void strangerVecAlert();
	void strangerDeqAlert();
	void insertVector();
	void insertDeque();

	size_t findVecRange(size_t idx) const;
	size_t findDeqRange(size_t idx) const;
	void printVector() const;
	void printDeque() const;
	void printArray(const std::string &word) const;
};

#endif // PMERGEME_HPP
