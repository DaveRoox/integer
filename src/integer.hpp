#ifndef INTEGER_HPP_
#define INTEGER_HPP_
#include <string>
#include <iostream>
#include <sstream>

namespace number {

	class integer final {

		class InvalidInputStringException : public std::exception {};
		class InvalidStateException : public std::exception {};

		class digit_node {
			short value;
			digit_node *next;
			digit_node(short, digit_node* = nullptr) noexcept; // Done
			~digit_node() noexcept; // Done
			friend class integer;
		};

		public:

			integer(const integer&) noexcept;  // Done

			integer() noexcept; // Done
			integer(short) noexcept;
			integer(int) noexcept; // Done

			integer(char); // Done
			integer(char*); // Done
			integer(const char*, int); // Done
			integer(char*, int); // Done
			integer(const std::string&); // Done

			~integer() noexcept; // Done

			/* Arithmetic operators */ //(Ripetere gli operatori con i tipi short, int, long, long long e corrispettivi unsigned + char, char* e string)
			integer& operator=(const integer&) noexcept; // Done
			integer operator+(const integer&) const; // Done
			integer operator+() const; // Done
			integer operator-(const integer&) const; // Done
			integer operator-() const; // Done
			integer operator*(const integer&) const; // Done
			integer operator/(const integer&) const;
			integer operator%(const integer&) const;
			integer operator^(const integer&) const; // Done /* Elevation to power */
			integer& operator++(); // Done
			integer operator++(int); // Done
			integer& operator--(); // Done
			integer operator--(int); // Done

			/* Comparison operators/relational operators */
			bool operator==(const integer&) const; // Done
			bool operator!=(const integer&) const; // Done
			bool operator>(const integer&) const; // Done
			bool operator<(const integer&) const; // Done
			bool operator>=(const integer&) const; // Done
			bool operator<=(const integer&) const; // Done

			/* Compound assignment operators */
			integer& operator+=(const integer&); // Done
			integer& operator-=(const integer&); // Done
			integer& operator*=(const integer&); // Done
			integer& operator/=(const integer&);
			integer& operator%=(const integer&);
			integer& operator^=(const integer&); //Done /* Elevation to power */

			void print(std::ostream& = std::cout) const noexcept; // Done
			std::string to_str() const noexcept; // Done

		private:

			digit_node *n;
			bool negative;
	};

	std::ostream& operator<<(std::ostream&, const integer&); // Done

}

#endif /* INTEGER_HPP_ */
