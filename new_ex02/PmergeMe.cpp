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
	std::vector<int> num;
	num.push_back(number);
	_vecExtra.push_back(num);
}

void PmergeMe::printVector() const {
	std::cout << BOLD BLU "A:  " RST;
	for (size_t i = 0; i < _vec.size(); ++i) {
		const std::vector<int>& first = _vec[i].first;
		std::cout << "[";
		for (size_t j = 0; j < first.size(); ++j) {
			if (first[j] < 10)
				std::cout << " ";
			std::cout << first[j];
			if (j + 1 < first.size())
				std::cout << ",";
		}
		std::cout << "]";
		if (i + 1 < _vec.size())
			std::cout << "  ";
	}
	std::cout << std::endl;

	std::cout << BOLD PNK "B:  " RST;
	for (size_t i = 0; i < _vec.size(); ++i) {
		const std::vector<int>& second = _vec[i].second;
		std::cout << "[";
		for (size_t j = 0; j < second.size(); ++j) {
			if (second[j] < 10)
				std::cout << " ";
			std::cout << second[j];
			if (j + 1 < second.size())
				std::cout << ",";
		}
		std::cout << "]";
		if (i + 1 < _vec.size())
			std::cout << "  ";
	}
	
	std::cout << RED;
	for (size_t i = 0; i < _vecExtra.size(); ++i) {
		const std::vector<int>& extra = _vecExtra[i];
		std::cout << "  [" ;
		for (size_t j = 0; j < extra.size(); ++j) {
			std::cout << extra[j];
			if (j + 1 < extra.size())
				std::cout << ",";
		}
		std::cout << "]";
	}
	std::cout << RST << std::endl;
}

void PmergeMe::compareVector() {
	for (size_t i = 0; i < _vec.size(); ++i) {
		std::vector<int>& first = _vec[i].first;
		std::vector<int>& second = _vec[i].second;
		if (first.empty() || second.empty())
			continue;
		vecComparisonCount++;
		if (first[0] < second[0]) {
			std::swap(first, second);
		}
	}
	std::cout << "Comparisons so far: " << vecComparisonCount << std::endl;
	printVector();
}

void PmergeMe::mergeVector() {
	for (size_t i = 0; i < _vec.size(); ++i) {
		if (!i && _vec[i].first.size() * 2 > initSize / 2)
			return (std::cout << "Finished merging!" << std::endl, merged = true, void());
		_vec[i].first.insert(_vec[i].first.end(),
					   		_vec[i].second.begin(),
					   		_vec[i].second.end());
		_vec[i].second.clear();
	}
	std::vector<std::pair<std::vector<int>, std::vector<int> > > tempVec;

	for (size_t i = 0; i < _vec.size(); i += 2) {
		if (i + 1 >= _vec.size())
			break;
		std::pair<std::vector<int>, std::vector<int> > pair;
		pair.first = _vec[i].first;
		pair.second = _vec[i + 1].first;
		tempVec.push_back(pair);
	}
	if (_vec.size() % 2)
		_vecExtra.insert(_vecExtra.begin(), _vec.back().first);
	

	_vec.swap(tempVec);
}

void PmergeMe::splitVector() {
	// size_t bucketSize = _numVector[0].size();
	//
	// sorted = 0;
	// toInsert = 0;
	// for (size_t i = 0; i < _numVector.size(); ++i) {
	// 	if (_numVector[i].size() == bucketSize) {
	// 		std::vector<int> left(_numVector[i].begin(), _numVector[i].begin() + bucketSize / 2);
	// 		std::vector<int> right(_numVector[i].begin() + bucketSize / 2, _numVector[i].end());
	// 		_sortedVector.push_back(right);
	// 		_numVector[i] = left;
	// 		sorted += 1;
	// 	}
	// }
	// for (size_t i = 0; i < _numVector.size(); i++ ) {
	// 	if (_numVector[i].size() == bucketSize / 2) {
	// 		toInsert++;
	// 	}
	// }

}

void PmergeMe::insertVector() {
	std::vector<size_t> insertionOrder = generateJacobsthalOrder(toInsert);

	// for (size_t inserted = 0; inserted < toInsert; inserted++) {
	// std::cout << "To insert: " << toInsert << " insertion order: ";
	// for (size_t i = 0; i < insertionOrder.size(); ++i) {
	// 	std::cout << insertionOrder[i] << " ";
	// }
	// std::cout << " already inserted: " << inserted << std::endl;
		// size_t idx = insertionOrder[inserted++];
	// 	if (idx < _numVector.size()) {
	// 		if (_numVector[idx].empty() || _numVector[idx].size() != _sortedVector[0].size())
	// 			continue;
	// 		if (idx == 0) {
	// 			_sortedVector.insert(_sortedVector.begin(), _numVector[idx]);
	// 			// _numVector.erase(_numVector.begin() + idx);
	// 			continue;
	// 		}
	// 		// Insert _numVector[idx] into _sortedVector
	// 		std::vector<int>& toInsertVec = _numVector[idx];
	// 		// Find correct position in _sortedVector
	// 		size_t pos = 0;
	// 		while (pos < _sortedVector.size() && _sortedVector[pos][0] < toInsertVec[0]) {
	// 			pos++;
	// 			vecComparisonCount++;
	// 		}
	// 		_sortedVector.insert(_sortedVector.begin() + pos, toInsertVec);
	// 	}
	// }

}

void PmergeMe::initVectorPairs() {
	for (size_t i = 0; i < _vecExtra.size(); i += 2) {
		if (i + 1 >= _vecExtra.size())
			break;
		std::vector<int> first = _vecExtra[i];
		std::vector<int> second = _vecExtra[i + 1];
		_vec.push_back(std::make_pair(first, second));
	}
	if (!_vecExtra.size() % 2)
		_vecExtra.clear();
	else
		_vecExtra.erase(_vecExtra.begin(), _vecExtra.end() - 1);
}

void PmergeMe::sort(std::string &input) {
	std::istringstream iss(input);
	int number;
	initSize = 0;
	while (iss >> number) {
		addNumber(number);
		initSize++;
	}
	initVectorPairs();
	printVector();

	merged = false;

	while (!merged) {
		compareVector();
		mergeVector();
	}
	// while (!merged) {
		// for (size_t i = 0; i < _numVector.size(); ++i) {
		// 	if (i + 1 < _numVector.size()) {
		// 		if (_numVector[i].size() == _numVector[i + 1].size()) {
		// 			if (_numVector[i][0] > _numVector[i + 1][0]) {
		// 				std::swap(_numVector[i], _numVector[i + 1]);
		// 				vecComparisonCount++;
		// 			}
		// 			if (_numVector[i].size() * 2 <= initSize / 2) {
		// 				_numVector[i].insert(_numVector[i].end(), _numVector[i + 1].begin(), _numVector[i + 1].end());
		// 				_numVector.erase(_numVector.begin() + i + 1);
		// 			} else {
		// 				merged = true;
		// 				break;
		// 			}
		// 		}
		// 	}
}
