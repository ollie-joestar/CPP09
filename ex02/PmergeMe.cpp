#include "PmergeMe.hpp"

#define RED "\033[31m"
#define YEL "\033[33m"
#define PNK "\033[35m"
#define BLU "\033[34m"
#define CYN "\033[36m"
#define GRN "\033[32m"
#define RST "\033[0m"
#define MAG "\033[95m"
#define BOLD "\033[1m"

PmergeMe::PmergeMe() : vecComparisonCount(0), dequeComparisonCount(0) {}

PmergeMe::~PmergeMe() {}

struct Pair {
	std::vector<int> first;
	std::vector<int> second;
	Pair(std::vector<int> f, std::vector<int> s) : first(f), second(s) {}
};

static std::vector<size_t> generateJacobsthalOrder(size_t n) {
    std::vector<size_t> order;
    if (n <= 0) return order;

    order.push_back(0); // first element is always index 0

    // Generate Jacobsthal numbers until >= n
    std::vector<size_t> J;
    J.push_back(1);
    J.push_back(1);
    while (J.back() < n) {
        int next = J[J.size()-1] + 2 * J[J.size()-2];
        J.push_back(next);
    }

    // Insert remaining indices
    std::vector<bool> used(n, false);
    used[0] = true;

    for (size_t k = 1; k < J.size(); ++k) {
        size_t start = J[k-1];
        size_t end   = J[k];
        if (start >= n) break;
        if (end > n) end = n;

        for (size_t i = end-1; i >= start; --i) {
            if (!used[i]) {
                order.push_back(i);
                used[i] = true;
            }
        }
    }

    // Add any missing indices at the end
    for (size_t i = 0; i < n; ++i) {
        if (!used[i]) order.push_back(i);
    }

    return order;
}

void PmergeMe::addNumber(int number) {
	std::vector<int> vec;
	vec.push_back(number);
	_numVector.push_back(vec);

	std::deque<int> lst;
	lst.push_back(number);
	_numDeque.push_back(lst);
}

void PmergeMe::printVector() const {
	std::cout << BOLD PNK "Vector contents: " RST;
	for (size_t i = 0; i < _numVector.size(); ++i) {
		std::cout << "[";
		for (size_t j = 0; j < _numVector[i].size(); ++j) {
			std::cout << _numVector[i][j];
			if (j < _numVector[i].size() - 1)
				std::cout << ",";
		}
		std::cout << "]";
	}
	std::cout << std::endl;
	std::cout << PNK "Sorted Vector contents: " RST;
	if (_sortedVector.empty()) {
		std::cout << "[]" << std::endl;
		return;
	}
	for (size_t i = 0; i < _sortedVector.size(); ++i) {
		std::cout << "[";
		for (size_t j = 0; j < _sortedVector[i].size(); ++j) {
			std::cout << _sortedVector[i][j];
			if (j < _sortedVector[i].size() - 1)
				std::cout << ",";
		}
		std::cout << "]";
	}
	std::cout << std::endl;
}

void PmergeMe::mergeVector() {
	for (size_t i = 0; i < _numVector.size(); ++i) {
		if (i + 1 < _numVector.size()) {
			if (_numVector[i].size() == _numVector[i + 1].size()) {
				if (_numVector[i][0] > _numVector[i + 1][0]) {
					std::swap(_numVector[i], _numVector[i + 1]);
					vecComparisonCount++;
				}
				if (_numVector[i].size() * 2 <= initSize / 2) {
					_numVector[i].insert(_numVector[i].end(), _numVector[i + 1].begin(), _numVector[i + 1].end());
					_numVector.erase(_numVector.begin() + i + 1);
				}
			}
		}
	}
}

void PmergeMe::splitVector() {
	size_t bucketSize = _numVector[0].size();

	sorted = 0;
	toInsert = 0;
	for (size_t i = 0; i < _numVector.size(); ++i) {
		if (_numVector[i].size() == bucketSize) {
			std::vector<int> left(_numVector[i].begin(), _numVector[i].begin() + bucketSize / 2);
			std::vector<int> right(_numVector[i].begin() + bucketSize / 2, _numVector[i].end());
			_sortedVector.push_back(right);
			_numVector[i] = left;
			sorted += 1;
		}
	}
	for (size_t i = 0; i < _numVector.size(); i++ ) {
		if (_numVector[i].size() == bucketSize / 2) {
			toInsert++;
		}
	}

}

void PmergeMe::insertVector() {
	std::vector<size_t> insertionOrder = generateJacobsthalOrder(toInsert);

	for (size_t inserted = 0; inserted < toInsert; inserted++) {
	std::cout << "To insert: " << toInsert << " insertion order: ";
	for (size_t i = 0; i < insertionOrder.size(); ++i) {
		std::cout << insertionOrder[i] << " ";
	}
	std::cout << " already inserted: " << inserted << std::endl;
		size_t idx = insertionOrder[inserted++];
		if (idx < _numVector.size()) {
			if (_numVector[idx].empty() || _numVector[idx].size() != _sortedVector[0].size())
				continue;
			if (idx == 0) {
				_sortedVector.insert(_sortedVector.begin(), _numVector[idx]);
				// _numVector.erase(_numVector.begin() + idx);
				continue;
			}
			// Insert _numVector[idx] into _sortedVector
			std::vector<int>& toInsertVec = _numVector[idx];
			// Find correct position in _sortedVector
			size_t pos = 0;
			while (pos < _sortedVector.size() && _sortedVector[pos][0] < toInsertVec[0]) {
				pos++;
				vecComparisonCount++;
			}
			_sortedVector.insert(_sortedVector.begin() + pos, toInsertVec);
		}
	}

}

void PmergeMe::sort(std::string &input) {
	std::istringstream iss(input);
	int number;
	initSize = 0;
	while (iss >> number) {
		addNumber(number);
		initSize++;
	}
	printVector();
	bool merged = false;

	while (!merged) {
		for (size_t i = 0; i < _numVector.size(); ++i) {
			if (i + 1 < _numVector.size()) {
				if (_numVector[i].size() == _numVector[i + 1].size()) {
					if (_numVector[i][0] > _numVector[i + 1][0]) {
						std::swap(_numVector[i], _numVector[i + 1]);
						vecComparisonCount++;
					}
					if (_numVector[i].size() * 2 <= initSize / 2) {
						_numVector[i].insert(_numVector[i].end(), _numVector[i + 1].begin(), _numVector[i + 1].end());
						_numVector.erase(_numVector.begin() + i + 1);
					} else {
						merged = true;
						break;
					}
				}
			}
		}
		std::cout << vecComparisonCount << " comparisons so far." << std::endl;
		printVector();
	}
		// mergeSortVector();
	// printDeque();
	// while (_numDeque[0].size() < static_cast<size_t>(initSize) / 2) {
	// 	mergeSortDeque();
	// 	std::cout << dequeComparisonCount << " comparisons so far." << std::endl;
	// 	printDeque();
	// }
	// while (_sortedVector.size() < static_cast<size_t>(initSize)) {
		splitVector();
		printVector();
		insertVector();
		printVector();
		// splitVector();
		// printVector();
		// std::cout << sorted << " sorted, " << toInsert << " to insert." << std::endl;
	// }
	// sortUsingVector();
	// sortUsingDeque();
}

// void PmergeMe::mergeInsertDeque() {
// 	size_t bucketSize = _numDeque[0].size();
//
// 	for (size_t i = 0; i < _numDeque.size(); ++i) {
// 		if (_numDeque[i].size() == bucketSize) {
// 			std::deque<int> left(_numDeque[i].begin(), _numDeque[i].begin() + bucketSize / 2);
// 			std::deque<int> right(_numDeque[i].begin() + bucketSize / 2, _numDeque[i].end());
// 			_numDeque[i] = left;
// 			_numDeque.insert(_numDeque.begin() + i + 1, right);
// 			i++;
// 		}
// 	}
// }
//
// void PmergeMe::printDeque() const {
// 	std::cout << BOLD BLU "Deque contents:  " RST;
// 	for (size_t i = 0; i < _numDeque.size(); ++i) {
// 		std::cout << "[";
// 		std::deque<int>::const_iterator it = _numDeque[i].begin();
// 		while (it != _numDeque[i].end()) {
// 			std::cout << *it;
// 			++it;
// 			if (it != _numDeque[i].end())
// 				std::cout << ",";
// 		}
// 		std::cout << "]";
// 	}
// 	std::cout << std::endl;
// }
//
// void PmergeMe::mergeSortDeque() {
// 	for (size_t i = 0; i < _numDeque.size(); ++i) {
// 		if (i + 1 < _numDeque.size()) {
// 			if (_numDeque[i].size() == _numDeque[i + 1].size()) {
// 				if (_numDeque[i][0] > _numDeque[i + 1][0]) {
// 					std::swap(_numDeque[i], _numDeque[i + 1]);
// 					dequeComparisonCount++;
// 				}
// 				_numDeque[i].insert(_numDeque[i].end(), _numDeque[i + 1].begin(), _numDeque[i + 1].end());
// 				_numDeque.erase(_numDeque.begin() + i + 1);
// 			}
// 		}
// 	}
// }
//
