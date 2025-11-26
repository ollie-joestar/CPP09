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

void PmergeMe::printDeque() const {
	std::cout << BOLD BLU "Deque contents:  " RST;
	for (size_t i = 0; i < _numDeque.size(); ++i) {
		std::cout << "[";
		std::deque<int>::const_iterator it = _numDeque[i].begin();
		while (it != _numDeque[i].end()) {
			std::cout << *it;
			++it;
			if (it != _numDeque[i].end())
				std::cout << ",";
		}
		std::cout << "]";
	}
	std::cout << std::endl;
}

void PmergeMe::mergeSortVector() {
	for (size_t i = 0; i < _numVector.size(); ++i) {
		if (i + 1 < _numVector.size()) {
			if (_numVector[i].size() == _numVector[i + 1].size()) {
				if (_numVector[i][0] > _numVector[i + 1][0]) {
					std::swap(_numVector[i], _numVector[i + 1]);
					vecComparisonCount++;
				}
				if (_numVector[i].size() > initSize / 2) {
					continue;
				}
				_numVector[i].insert(_numVector[i].end(), _numVector[i + 1].begin(), _numVector[i + 1].end());
				_numVector.erase(_numVector.begin() + i + 1);
			}
		}
	}
}

void PmergeMe::mergeSortDeque() {
	for (size_t i = 0; i < _numDeque.size(); ++i) {
		if (i + 1 < _numDeque.size()) {
			if (_numDeque[i].size() == _numDeque[i + 1].size()) {
				if (_numDeque[i][0] > _numDeque[i + 1][0]) {
					std::swap(_numDeque[i], _numDeque[i + 1]);
					dequeComparisonCount++;
				}
				_numDeque[i].insert(_numDeque[i].end(), _numDeque[i + 1].begin(), _numDeque[i + 1].end());
				_numDeque.erase(_numDeque.begin() + i + 1);
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
	for (size_t i = 0; i < _numVector.size(); i += 2) {
		if (_numVector[i].size() == bucketSize / 2) {
			toInsert++;
		}
	}

}
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

void PmergeMe::mergeInsertVector() {
	std::vector<size_t> insertionOrder = generateJacobsthalOrder(toInsert);
}

void PmergeMe::mergeInsertDeque() {
	size_t bucketSize = _numDeque[0].size();

	for (size_t i = 0; i < _numDeque.size(); ++i) {
		if (_numDeque[i].size() == bucketSize) {
			std::deque<int> left(_numDeque[i].begin(), _numDeque[i].begin() + bucketSize / 2);
			std::deque<int> right(_numDeque[i].begin() + bucketSize / 2, _numDeque[i].end());
			_numDeque[i] = left;
			_numDeque.insert(_numDeque.begin() + i + 1, right);
			i++;
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
	while (_numVector[0].size() < static_cast<size_t>(initSize) / 2) {
		mergeSortVector();
		std::cout << vecComparisonCount << " comparisons so far." << std::endl;
		printVector();
	}
	// printDeque();
	// while (_numDeque[0].size() < static_cast<size_t>(initSize) / 2) {
	// 	mergeSortDeque();
	// 	std::cout << dequeComparisonCount << " comparisons so far." << std::endl;
	// 	printDeque();
	// }
	// while (_numVector.size() < static_cast<size_t>(initSize)) {
		splitVector();
		printVector();
		splitVector();
		printVector();
		std::cout << sorted << " sorted, " << toInsert << " to insert." << std::endl;
	// }
	// sortUsingVector();
	// sortUsingDeque();
}
