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
	std::vector<int> numVec;
	numVec.push_back(number);
	_vec.push_back(numVec);

	std::deque<int> numDeq;
	numDeq.push_back(number);
	_deq.push_back(numDeq);
}

void PmergeMe::printArray(const std::string &word) const {
	std::cout << word << ": ";
	for (size_t i = 0; i < _vec.size(); ++i) {
		std::cout << _vec[i][0];
		if (i + 1 < _vec.size())
			std::cout << " ";
	}
	std::cout << std::endl;
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

void PmergeMe::printDeque() const {
	std::cout << GRN "S:  " RST;
	for (size_t i = 0; i < _deqSorted.size(); ++i) {
		const std::deque<int>& sorted = _deqSorted[i];
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
	for (size_t i = 0; i < _deq.size(); ++i) {

		const std::deque<int>& curr = _deq[i];
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
	for (size_t i = 0; i < _deqInsert.size(); ++i) {
		const std::deque<int>& curr = _deqInsert[i];
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
	for (size_t i = 0; i < _deqExtra.size(); ++i) {
		const std::deque<int>& extra = _deqExtra[i];
		std::cout << "[";
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
}

void PmergeMe::compareDeque() {
	for (size_t i = 0; i < _deq.size(); i += 2) {
		if (i + 1 >= _deq.size())
			break;
		std::deque<int>& left = _deq[i];
		std::deque<int>& right = _deq[i + 1];
		dequeComparisonCount++;
		if (left[left.size() - 1] > right[right.size() - 1]) {
			std::swap(left, right);
		}
	}
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
	if (_vec[0].size() * 2 > initSize / 2) {
		merged = true;
		return;
	}
}

void PmergeMe::mergeDeque() {
	for (size_t i = 0; i < _deq.size(); ++i) {
		if (i + 1 >= _deq.size()) {
			_deqExtra.insert(_deqExtra.begin(), _deq[i]);
			_deq.erase(_deq.begin() + i);
			break;
		}
		std::deque<int> left = _deq[i];
		std::deque<int> right = _deq[i + 1];
		left.insert(left.end(), right.begin(), right.end());
		_deq[i] = left;
		_deq.erase(_deq.begin() + i + 1);
	}
	if (_deq[0].size() * 2 > initSize / 2) {
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

void PmergeMe::splitDeque() {
	toInsert = 0;
	size_t half = _deq[0].size() / 2;
	for (size_t i = 0; i < _deq.size(); ++i) {
		std::deque<int> first;
		first.insert(first.end(), _deq[i].begin(), _deq[i].begin() + half);
		std::deque<int> second;
		second.insert(second.end(), _deq[i].begin() + half, _deq[i].end());
		_deqInsert.push_back(first);
		_deq[i] = second;
		_deqSorted.push_back(second);
		toInsert++;
	}
	if (!_deqExtra.empty()) {
		std::deque<int> stranger = _deqExtra[0];
		if (stranger.size() == half) {
			_deqInsert.push_back(stranger);
			toInsert++;
			_deqExtra.erase(_deqExtra.begin());
		}
	}
}

size_t PmergeMe::findVecRange(size_t idx) const {
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

size_t PmergeMe::findDeqRange(size_t idx) const {
	if (!idx)
		return 0;
	if (idx >= _deq.size()) {
		return _deqSorted.size() - 1;
	}
	for (size_t i = idx; i < _deqSorted.size(); ++i)
		if (_deqSorted[i] == _deq[idx]) {
			return i - 1;
		}
	return _deqSorted.size() - 1;
}

static void printVecInDB(size_t begin, size_t pos, size_t end, const std::vector<std::vector<int> >& S, std::string color) {
		std::cout << color <<"Begin: " << begin << " End: " << end << " Pos: " << pos << RST<< std::endl;
		for (size_t p = 0; p < S.size(); ++p) {
			p == begin ? std::cout << CYN "{" RST : std::cout << " ";
			std::cout << " ";
			p == pos ? std::cout << YEL "V" RST : std::cout << " ";
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

static void printDeqInDB(size_t begin, size_t pos, size_t end, const std::deque<std::deque<int> >& S, std::string color) {
		std::cout << color <<"Begin: " << begin << " End: " << end << " Pos: " << pos << RST<< std::endl;
		for (size_t p = 0; p < S.size(); ++p) {
			p == begin ? std::cout << CYN "{" RST : std::cout << " ";
			std::cout << " ";
			p == pos ? std::cout << YEL "V" RST : std::cout << " ";
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
		size_t idx = insertionOrder[i];
		size_t end = findVecRange(idx);
		if (!idx) {
			_vecSorted.insert(_vecSorted.begin(), _vecInsert[0]);
			continue;
		}
		std::vector<int> homeless = _vecInsert[idx];
		// Find correct position in _vecSorted
		size_t begin = 0;
		size_t pos = (end - begin) / 2;
		// Binary search for insertion point
		while (begin <= end) {
			// printVecInDB(begin, pos, end, _vecSorted, RST);
			if (homeless[homeless.size() - 1] < _vecSorted[pos][_vecSorted[pos].size() - 1]) {
				vecComparisonCount++;
				if (begin >= end) {
					_vecSorted.insert(_vecSorted.begin() + pos, homeless);
					break;
				}
				end = pos - 1 * (pos > 0);
				pos = begin + (end - begin) / 2;
				continue;
			} else if (homeless[homeless.size() - 1] > _vecSorted[pos][_vecSorted[pos].size() - 1]) {
				vecComparisonCount++;
				if (begin == end) {
					_vecSorted.insert(_vecSorted.begin() + begin + 1, homeless);
					break;
				}
				begin = pos + 1;
				pos = begin + (end - begin) / 2;
				continue;
			} else {
				vecComparisonCount++;
				begin = pos + 1;
				pos = begin + (end - begin) / 2;
				if (begin >= end) {
					printVecInDB(begin, pos, end, _vecSorted, RED);
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
}

void PmergeMe::insertDeque () {
	std::vector<size_t> insertionOrder = generateJacobsthalOrder(toInsert);

	for (size_t i = 0; i < toInsert; i++) {
		size_t idx = insertionOrder[i];
		size_t end = findDeqRange(idx);
		if (!idx) {
			_deqSorted.insert(_deqSorted.begin(), _deqInsert[0]);
			continue;
		}
		std::deque<int> homeless = _deqInsert[idx];
		// Find correct position in _deqSorted
		size_t begin = 0;
		size_t pos = (end - begin) / 2;
		// Binary search for insertion point
		while (begin <= end) {
			// printDeqInDB(begin, pos, end, _deqSorted, RST);
			if (homeless[homeless.size() - 1] < _deqSorted[pos][_deqSorted[pos].size() - 1]) {
				dequeComparisonCount++;
				if (begin >= end) {
					_deqSorted.insert(_deqSorted.begin() + pos, homeless);
					break;
				}
				end = pos - 1 * (pos > 0);
				pos = begin + (end - begin) / 2;
				continue;
			} else if (homeless[homeless.size() - 1] > _deqSorted[pos][_deqSorted[pos].size() - 1]) {
				dequeComparisonCount++;
				if (begin == end) {
					_deqSorted.insert(_deqSorted.begin() + begin + 1, homeless);
	  					break;
	  				}
	  				begin = pos + 1;
	  				pos = begin + (end - begin) / 2;
	  				continue;
	  		} else {
	  			dequeComparisonCount++;
	  			begin = pos + 1;
	  			pos = begin + (end - begin) / 2;
	  			if (begin >= end) {
	  				printDeqInDB(begin, pos, end, _deqSorted, RED);
	  				_deqSorted.insert(_deqSorted.begin() + begin, homeless);
	  				break;
	  			}
	  			continue;
	  		}
	  	}
	}
	_deqInsert.clear();
	_deq.swap(_deqSorted);
	_deqSorted.clear();
}

void PmergeMe::sortUsingVector() {
	struct timeval start, end;
	gettimeofday(&start, NULL);

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
	while (_vec[0].size() > 1) {
		splitVector();
		insertVector();
		if (vecComparisonCount < 0) {
			std::cout << "Too many comparisons, stopping..." << std::endl;
			return;
		}
	}
	gettimeofday(&end, NULL);

    long seconds  = end.tv_sec  - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;
    long total_us = seconds * 1000000 + useconds;

	vecTime = total_us;
	// std::cout << "Total vec comparisons: " << vecComparisonCount << std::endl;
}

void PmergeMe::sortUsingDeque() {
	struct timeval start, end;
	gettimeofday(&start, NULL);

	merged = false;
	while (!merged) {
		compareDeque();
		if (merged)
			break;
		mergeDeque();
	}
	dequeComparisonCount++;
	if (_deq[0][_deq[0].size() - 1] > _deq[1][_deq[1].size() - 1]) {
		std::swap(_deq[0], _deq[1]);
	}
	while (_deq[0].size() > 1) {
		splitDeque();
		insertDeque();
		if (dequeComparisonCount < 0) {
			std::cout << "Too many comparisons, stopping..." << std::endl;
			return;
		}
	}
	gettimeofday(&end, NULL);

	long seconds  = end.tv_sec  - start.tv_sec;
	long useconds = end.tv_usec - start.tv_usec;
	long total_us = seconds * 1000000 + useconds;

	deqTime = total_us;
	// std::cout << "Total deq comparisons: " << dequeComparisonCount << std::endl;
}

void PmergeMe::sort(std::string &input)
{
    std::istringstream iss(input);
    int number;
    initSize = 0;

    while (true)
    {
        if (iss >> number) {
			if (number < 0)
				throw std::runtime_error("Error: negative number in input");
            addNumber(number);
            initSize++;
        } else {
            if (iss.eof())
                break;
            throw std::runtime_error("Error: non-numeric value in input");
        }
    }

	printArray("Before");
	sortUsingDeque();
    sortUsingVector();
	printArray("After");
    std::cout << "Time to process a range of "
              << initSize
              << " elements with std::vector : "
              << vecTime
              << " us"
              << std::endl;
	std::cout << "Time to process a range of "
			  << initSize
			  << " elements with std::deque  : "
			  << deqTime
			  << " us"
			  << std::endl;
}
