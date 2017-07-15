#include "rights.h"
#include <stdexcept>
#include <exception>

std::string AccessRight::str() {
	std::string right = std::string();
	int r = _right;
	if (r % 2) {
		right.insert(0, "x");
	} else {
		right.insert(0, "-");
	}
	r /= 2;
	if (r % 2) {
		right.insert(0, "w");
	} else {
		right.insert(0, "-");
	}
	r /= 2;
	if (r % 2) {
		right.insert(0, "r");
	} else {
		right.insert(0, "-");
	}
	return right;
}

AccessRight::AccessRight(std::string right) {
	switch (right[0]) {
		case 'r':
			_right = 4;
			break;
		case '-':
			_right = 0;
			break;
		default:
			throw std::invalid_argument(right);
	}
	switch (right[1]) {
		case 'w':
			_right += 2;
			break;
		case '-':
			break;
		default:
			throw std::invalid_argument(right);
	}
	switch (right[2]) {
		case 'x':
			_right += 1;
			break;
		case '-':
			break;
		default:
			throw std::invalid_argument(right);
	}
}

void AccessRight::change(std::string right) {
	int r, i = 1;
	int add = 0;

	switch (right[0]) {
		case '+':
			add = 1;
			r = 0;
			break;
		case '-':
			r = 7;
			break;
		default:
			throw std::invalid_argument(right);
	}
	while(right[i]) {
		switch (right[i]) {
			case 'r':
				add ? r |= 4 : r &= 3; 
				break;
			case 'w':
				add ? r |= 2 : r &= 5; 
				break;
			case 'x':
				add ? r |= 1 : r &= 6; 
				break;
			default:
				throw std::invalid_argument(right);
		}
		i++;
	}
	add ? _right |= r : _right &= r;
}