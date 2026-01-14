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
	_vec.push_back(num);
}

void PmergeMe::printVector() const {
	std::cout << GRN "S:  " RST;
	for (size_t i = 0; i < _vecSorted.size(); ++i) {
		const std::vector<int>& sorted = _vecSorted[i];
		std::cout << "[";
		for (size_t j = 0; j < sorted.size(); ++j) {
			if (sorted[j] < 10)
				std::cout << " ";
			std::cout << sorted[j];
			if (j + 1 < sorted.size())
				std::cout << ",";
		}
		std::cout << "]";
	}
	std::cout << std::endl;

	std::cout << BOLD BLU "A:  " RST;
	for (size_t i = 0; i < _vec.size(); ++i) {

		const std::vector<int>& curr = _vec[i];
		std::cout << "[";
		for (size_t j = 0; j < curr.size(); ++j) {
			if (curr[j] < 10)
				std::cout << " ";
			std::cout << curr[j];
			if (j + 1 < curr.size())
				std::cout << ",";
		}
		std::cout << "]";
	}
	std::cout << std::endl;

	std::cout << BOLD PNK "B:  " RST;
	for (size_t i = 0; i < _vecInsert.size(); ++i) {
		const std::vector<int>& curr = _vecInsert[i];
		std::cout << "[";
		for (size_t j = 0; j < curr.size(); ++j) {
			if (curr[j] < 10)
				std::cout << " ";
			std::cout << curr[j];
			if (j + 1 < curr.size())
				std::cout << ",";
		}
		std::cout << "]";
	}
	std::cout << std::endl;
	
	std::cout << BOLD RED "E:  " RST;
	for (size_t i = 0; i < _vecExtra.size(); ++i) {
		const std::vector<int>& extra = _vecExtra[i];
		std::cout << "[" ;
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
	for (size_t i = 0; i < _vec.size(); i += 2) {
		if (i + 1 >= _vec.size())
			break;
		std::vector<int>& left = _vec[i];
		std::vector<int>& right = _vec[i + 1];
		vecComparisonCount++;
		if (left[left.size() - 1] > right[right.size() - 1]) {
			std::swap(left, right);
		}
	}
	std::cout << "Comparisons so far: " << vecComparisonCount << std::endl;
	printVector();
	std::cout << std::endl;
}

void PmergeMe::mergeVector() {
	for (size_t i = 0; i < _vec.size(); ++i) {
		if (i + 1 >= _vec.size()) {
			_vecExtra.insert(_vecExtra.begin(), _vec[i]);
			_vec.erase(_vec.begin() + i);
			break;
		}
		std::vector<int> left = _vec[i];
		std::vector<int> right = _vec[i + 1];
		left.insert(left.end(), right.begin(), right.end());
		_vec[i] = left;
		_vec.erase(_vec.begin() + i + 1);
	}
	std::cout << "After merging: " << std::endl;
	printVector();
	std::cout << std::endl;
	if (_vec[0].size() * 2 > initSize / 2) {
		std::cout << "Finished merging!" << std::endl;
		merged = true;
		return;
	}
}

void PmergeMe::splitVector() {
	toInsert = 0;
	size_t half = _vec[0].size() / 2;
	for (size_t i = 0; i < _vec.size(); ++i) {
		std::vector<int> first;
		first.insert(first.end(), _vec[i].begin(), _vec[i].begin() + half);
		std::vector<int> second;
		second.insert(second.end(), _vec[i].begin() + half, _vec[i].end());
		_vecInsert.push_back(first);
		_vec[i] = second;
		_vecSorted.push_back(second);
		toInsert++;
	}
	if (!_vecExtra.empty()) {
		std::vector<int> stranger = _vecExtra[0];
		if (stranger.size() == half) {
			_vecInsert.push_back(stranger);
			toInsert++;
			_vecExtra.erase(_vecExtra.begin());
		}
	}
}

size_t PmergeMe::findRange(size_t idx) const {
	if (!idx)
		return 0;
	if (idx >= _vec.size()) {
		return _vecSorted.size() - 1;
	}
	for (size_t i = idx; i < _vecSorted.size(); ++i)
		if (_vecSorted[i] == _vec[idx]) {
			return i - 1;
		}
	return _vecSorted.size() - 1;
}

static void printInsertion(size_t begin, size_t pos, size_t end, const std::vector<std::vector<int> >& S, std::string color) {
		std::cout << color <<"Begin: " << begin << " End: " << end << " Pos: " << pos << RST<< std::endl;
		for (size_t p = 0; p < S.size(); ++p) {
			p == begin ? std::cout << CYN "{" RST : std::cout << " ";
			std::cout << " ";
			p == pos ? std::cout << YEL "V" RST : std::cout << " ";
			// if (p != pos && (p >= begin && p <= end))
			// 	std::cout << BLU "^" RST;
			// else
			// 	std::cout << " ";
			std::cout << " ";
			p == end ? std::cout << CYN "}" RST : std::cout << " ";
			std::cout << " ";
		}
		std::cout << std::endl;
		for (size_t p = 0; p < S.size(); ++p) {
			std::cout << " [";
			if (S[p][S[p].size() - 1] < 10)
				std::cout << " ";
			std::cout << S[p][S[p].size() - 1] << "]";
			std::cout << " ";
		}
		std::cout << std::endl;
}

void PmergeMe::insertVector() {
	std::vector<size_t> insertionOrder = generateJacobsthalOrder(toInsert);

	for (size_t i = 0; i < toInsert; i++) {
		std::cout << RED"											Comparisons so far: " << vecComparisonCount << RST << std::endl;
		size_t idx = insertionOrder[i];
		size_t end = findRange(idx);
		if (!idx) {
			_vecSorted.insert(_vecSorted.begin(), _vecInsert[0]);
			continue;
		}
		std::vector<int> homeless = _vecInsert[idx];
		// Find correct position in _vecSorted
		size_t begin = 0;
		size_t pos = (end - begin) / 2;
		// Binary search for insertion point
		std::cout << std::endl;
		std::cout << "Searching for [" ;
		if (homeless[homeless.size() - 1] < 10)
			std::cout << " ";
		std::cout << homeless[homeless.size() - 1] << "] in S: ";
		std::cout << std::endl;
		while (begin <= end) {
		// for (size_t i = 0;i < 10; ++i) {
			printInsertion(begin, pos, end, _vecSorted, RST);
			if (homeless[homeless.size() - 1] < _vecSorted[pos][_vecSorted[pos].size() - 1]) {
				vecComparisonCount++;
				if (begin >= end) {
					// std::cout << YEL"Begin: " << begin << " End: " << end << " Pos: " << pos << RST<< std::endl;
					// printInsertion(begin, pos, end, _vecSorted, GRN);
					_vecSorted.insert(_vecSorted.begin() + pos, homeless);
					break;
				}
				end = pos - 1 * (pos > 0);
				pos = begin + (end - begin) / 2;
				// if (begin >= end) {
				// 	printInsertion(begin, pos, end, _vecSorted, MAG);
				// 	_vecSorted.insert(_vecSorted.begin() + begin, homeless);
				// 	break;
				// }
				continue;
			} else if (homeless[homeless.size() - 1] > _vecSorted[pos][_vecSorted[pos].size() - 1]) {
				vecComparisonCount++;
				if (begin == end) {
					// printInsertion(begin, pos, end, _vecSorted, PNK);
					_vecSorted.insert(_vecSorted.begin() + begin + 1, homeless);
					break;
				}
				begin = pos + 1;
				pos = begin + (end - begin) / 2;
				// if (begin >= end) {
				// 	printInsertion(begin, pos, end, _vecSorted, YEL);
				// 	_vecSorted.insert(_vecSorted.begin() + begin + 1, homeless);
				// 	break;
				// }
				continue;
			} else {
				vecComparisonCount++;
				begin = pos + 1;
				pos = begin + (end - begin) / 2;
				if (begin >= end) {
					printInsertion(begin, pos, end, _vecSorted, RED);
					_vecSorted.insert(_vecSorted.begin() + begin, homeless);
					break;
				}
				continue;
			}
		}
	}
	_vecInsert.clear();
	_vec.swap(_vecSorted);
	_vecSorted.clear();
	printVector();
	std::cout << RED"											Comparisons so far: " << vecComparisonCount << RST << std::endl;
	std::cout << "Comparisons so far: " << vecComparisonCount << std::endl;
}

// Jacpb number 1 3 2   5 4   11 10    9     8     7     6
// search range 0 3 2-3 7 6-7 15 14-15 13-14 12-13 11-12 10-11
// comparisons  0 2 2   3 3   4  4     4     4     4     4 

void PmergeMe::sort(std::string &input) {
	std::istringstream iss(input);
	int number;
	initSize = 0;
	while (iss >> number) {
		addNumber(number);
		initSize++;
	}
	printVector();

	merged = false;
	while (!merged) {
		compareVector();
		if (merged)
			break;
		mergeVector();
	}
	vecComparisonCount++;
	if (_vec[0][_vec[0].size() - 1] > _vec[1][_vec[1].size() - 1]) {
		std::swap(_vec[0], _vec[1]);
	}
	std::cout << "Comparisons so far: " << vecComparisonCount << std::endl;
	std::cout << GRN"--------------------------" << std::endl;
	std::cout << GRN"--------------------------" << std::endl;
	std::cout << GRN"--------------------------" << std::endl;
	printVector();
	std::cout << GRN"--------------------------" << std::endl;
	std::cout << GRN"--------------------------" << std::endl;
	std::cout << GRN"--------------------------" << RST << std::endl;
	while (_vec[0].size() > 1) {
		std::cout << "--------------------------" << std::endl;
		splitVector();
		printVector();
		insertVector();
		if (vecComparisonCount > 90) {
			std::cout << "Too many comparisons, stopping..." << std::endl;
			return;
		}
	}
	std::cout << GRN BOLD "GG WP! Final sorted array: " << std::endl;
	for (size_t i = 0; i < _vec.size(); ++i) {
		std::cout << "[" << _vec[i][0] << "]";
	}
	std::cout << RST << std::endl;
	// for (int i = 0; i < 2; ++i) {
	// 	std::cout << "--------------------------" << std::endl;
	// 	splitVector();
	// 	insertVector();
	// }
	// insertVector();
}
