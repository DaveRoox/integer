#include "integer.hpp"

#include <stack>

/* ----------------------------- C O N S T R U C T O R S ----------------------------- */

integer::digit_node::digit_node(short _value, digit_node *_next) noexcept: value(_value), next(_next) {}

integer::integer(const integer &_number) noexcept {
	n = nullptr;
	negative = false;
	*this = _number;
}

integer::integer() noexcept {
	n = nullptr;
	negative = false;
}

integer::integer(int _value) noexcept {

	if(_value < 0)
		negative = true, _value *= -1;
	else
		negative = false;

	digit_node *nn = n = new digit_node(_value % 10);

	while(_value /= 10)
		nn = nn->next = new digit_node(_value % 10);

}

integer::integer(char c) {

	if(c < '0' or c > '9')
		throw integer::InvalidInputStringException();

	n = new digit_node(c - '0');
	negative = false;

}

integer::integer(const char *s, int size) {

	if(size == 0)
		return;

	int idx = negative = s[0] == '-';

	while(idx < (size - 1) and s[idx] == '0')
		idx++;

	digit_node *nn = this->n = nullptr;

	char c;
	while(idx < size and '0' <= (c = s[idx]) and c <= '9')
		nn = new digit_node(c - '0', nn), idx++;

	if(idx < size or nn == nullptr)
		throw integer::InvalidInputStringException();

	this->n = nn;

}

integer::integer(const std::string &s): integer(s.c_str(), s.size()) {}
integer::integer(char *s, int size): integer((const char*)s, size) {}
integer::integer(char *s): integer(s, strlen(s)) {}

/* ------------------------------- D E S T R U C T O R ------------------------------- */

integer::digit_node::~digit_node() noexcept {
	if(next != nullptr)
		delete next;
}

integer::~integer() noexcept {
	if(n != nullptr)
		delete n;
}

/* -------------------------------- O P E R A T O R S -------------------------------- */

integer& integer::operator=(const integer &_number) noexcept {

	if(&_number == this)
		return *this;

	if(_number.n == nullptr) {

		if(this->n != nullptr)
			delete this->n;

		this->n = nullptr;
		this->negative = false;

		return *this;
	}

	this->negative = _number.negative;

	if(this->n == nullptr)
		n = new digit_node(_number.n->value);
	else
		n->value = _number.n->value;

	digit_node *nn = this->n;
	digit_node *_nn = _number.n;

	while(nn->next != nullptr and _nn->next != nullptr) {
		nn->next->value = _nn->next->value;
		nn = nn->next, _nn = _nn->next;
	}

	if(nn->next != nullptr) {
		delete nn->next;
		nn->next = nullptr;
	}
	else if(_nn->next != nullptr) {
		nn->next = new digit_node(_nn->next->value);
		nn = nn->next, _nn = _nn->next->next;
		while(_nn != nullptr) {
			nn->next = new digit_node(_nn->value);
			nn = nn->next, _nn = _nn->next;
		};
	}

	return *this;
}

// (+) - (+) or (-) - (-)
// while (+) - (-) is converted to (+) + (+) and (-) - (+) is converted to (-) + (-)
integer integer::operator-(const integer &_number) const {

	if(this->n == nullptr or _number.n == nullptr)
		throw integer::InvalidStateException();

	if(this == &_number or *this == _number)
		return integer{0};

	if(this->negative != _number.negative)
		return *this + -_number;

	if((this->negative and *this > _number) or (!this->negative and *this < _number))
		return -(_number - *this);

	integer result;

	short tmp = this->n->value - _number.n->value;
	short carry = tmp < 0;

	result.negative = this->negative;
	result.n = new digit_node(tmp + 10 * carry);

	digit_node *first = this->n->next;
	digit_node *second = _number.n->next;
	digit_node *third = result.n;

	while(first != nullptr and second != nullptr) {
		tmp = first->value - second->value - carry;
		carry = tmp < 0;
		third = third->next = new digit_node(tmp + 10 * carry);
		first = first->next, second = second->next;
	}

	while(first != nullptr) {
		tmp = first->value - carry;
		carry = tmp < 0;
		third = third->next = new digit_node(tmp + 10 * carry);
		first = first->next;
	}

	if(carry > 0) {
		third->value = 10 - third->value;
		result.negative = !result.negative;
		carry = 0;
	}

	third = result.n;

	std::stack<digit_node*> _stck;

	while(third != nullptr)
		_stck.push(third), third = third->next;

	while(!_stck.empty() and _stck.top()->value == 0)
		_stck.pop();

	if(!_stck.empty()) {
		digit_node *last = _stck.top();
		delete last->next;
		last->next = nullptr;
	}

	return result;

}

// (+) + (+) or (-) + (-)
// while (+1) + (-2) is converted to (+1) - (+2) and (-1) + (+2) is converted to (+2) - (+1)
integer integer::operator+(const integer &_number) const {

	if(this->n == nullptr or _number.n == nullptr)
		throw integer::InvalidStateException();

	if(this->negative != _number.negative)
		return this->negative? _number - -*this : *this - -_number;

	integer result;

	short tmp = this->n->value + _number.n->value;
	short carry = tmp / 10;

	result.negative = this->negative and _number.negative;
	result.n = new digit_node(tmp % 10);

	digit_node *first = this->n->next;
	digit_node *second = _number.n->next;
	digit_node *third = result.n;

	while(first != nullptr and second != nullptr) {
		tmp = first->value + second->value + carry;
		carry = tmp / 10;
		third = third->next = new digit_node(tmp % 10);
		first = first->next, second = second->next;
	}

	while(first != nullptr) {
		tmp = first->value + carry;
		carry = tmp / 10;
		third = third->next = new digit_node(tmp % 10);
		first = first->next;
	}

	while(second != nullptr) {
		tmp = second->value + carry;
		carry = tmp / 10;
		third = third->next = new digit_node(tmp % 10);
		second = second->next;
	}

	if(carry > 0)
		third->next = new digit_node(carry);

	return result;

}

bool integer::operator==(const integer &_number) const {

	if(this == &_number)
		return true;

	digit_node *first = this->n;
	digit_node *second = _number.n;

	while(first != nullptr and second != nullptr) {
		if(first->value != second->value)
			return false;
		first = first->next, second = second->next;
	}

	return first == nullptr and second == nullptr;
}

bool integer::operator<(const integer &_number) const {

	if(this == &_number)
		return false;

	if(this->n == nullptr or _number.n == nullptr)
		throw integer::InvalidStateException();

	if(this->negative != _number.negative)
		return this->negative;

	std::stack<short> first, second;

	digit_node *ndfirst = this->n;
	digit_node *ndsecond = _number.n;

	while(ndfirst != nullptr)
		first.push(ndfirst->value), ndfirst = ndfirst->next;

	while(ndsecond != nullptr)
		second.push(ndsecond->value), ndsecond = ndsecond->next;

	if(first.size() != second.size())
		return (first.size() > second.size() and this->negative) or (first.size() < second.size() and !this->negative);

	while(!first.empty() and first.top() == second.top())
		first.pop(), second.pop();

	return first.empty() ? false : (first.top() < second.top() and !this->negative) or (first.top() > second.top() and this->negative);

}

bool integer::operator>=(const integer &_number) const {
	return !(*this < _number);
}

bool integer::operator<=(const integer &_number) const {
	return *this == _number or *this < _number;
}

bool integer::operator>(const integer &_number) const {
	return *this >= _number and *this != _number;
}

bool integer::operator!=(const integer &_number) const {
	return !(*this == _number);
}

integer integer::operator+() const {
	return *this;
}

integer integer::operator-() const {
	integer result(*this);
	result.negative = !this->negative;
	return result;
}

integer& integer::operator++() {
	return *this += 1;
}

integer integer::operator++(int) {
	return *this += 1;
}

integer& integer::operator--() {
	return *this -= 1;
}

integer integer::operator--(int) {
	return *this -= 1;
}

integer& integer::operator+=(const integer &_number) {
	return *this = *this + _number;
}

integer& integer::operator-=(const integer &_number) {
	return *this = *this - _number;
}

/* ---------------------------------- M E T H O D S ---------------------------------- */

void integer::print(std::ostream &out) const noexcept {
	out << this->to_str();
}

std::string integer::to_str() const noexcept {

	std::stringstream ss;

	if(negative)
		ss << '-';

	std::stack<short> _stck;
	digit_node *nn = this->n;

	while(nn != nullptr)
		_stck.push(nn->value), nn = nn->next;

	while(!_stck.empty())
		ss << _stck.top(), _stck.pop();

	return ss.str();
}

/* -------------------------------- F U N C T I O N S -------------------------------- */

std::ostream& operator<<(std::ostream &out, const integer &number) {
	return out << number.to_str();
}
