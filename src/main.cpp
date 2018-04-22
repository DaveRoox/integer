#include <iostream>
#include <iomanip>
#include "integer.hpp"
#include <ctime>

using namespace std;

template<class T, typename K> struct Sub {
	T operator()(T & i1, T & i2) {
		return i1 - i2;
	};
	bool compare(integer & t, long long & k) {
		return t.to_str() == to_string(k);
	}
};

template<class T, class K> struct Add {
	T operator()(T & i1, T & i2) {
		return i1 + i2;
	};
	bool compare(integer & t, long long & k) {
		return t.to_str() == to_string(k);
	}
};

template<class T, class K> struct LEQ {
	bool operator()(T & i1, T & i2) {
		return i1 <= i2;
	};
};

template<class T, class K> struct Less {
	bool operator()(T & i1, T & i2) {
		return i1 < i2;
	};
};

template<class T, class K> struct GEQ {
	bool operator()(T & i1, T & i2) {
		return i1 >= i2;
	};
};

template<class T, class K> struct Greater {
	bool operator()(T & i1, T & i2) {
		return i1 > i2;
	};
};

template<class T, class K> struct EQU {
	bool operator()(T & i1, T & i2) {
		return i1 == i2;
	};
};

template<class T, class K> struct NEQ {
	bool operator()(T & i1, T & i2) {
		return i1 != i2;
	};
};

template<template<typename T, typename K> class functor>
void test(const long long N_TESTS, char *msg) {
	cout << msg << " (" << N_TESTS << " casi di test)\n";
	long long ok = 0;
	clock_t start = clock();
	for(long long i = 0; i < N_TESTS; i++) {
		long long first = rand() % 9223372036854775807LL - rand() % 9223372036854775807LL;
		long long second = rand() % 9223372036854775807LL - rand() % 9223372036854775807LL;
		auto result = functor<long long, integer>()(first, second);
		integer nfirst{to_string(first)}, nsecond{to_string(second)};
		auto nresult = functor<integer, long long>()(nfirst, nsecond);
		if(!functor<int, int>().compare(nresult, result)) {
			cerr << "Error:\n";
			cerr << "first: " << first << endl << "second: " << second << endl << "nfirst: " << nfirst << endl << "nsecond: " << nsecond << endl << endl;
			cerr << "Expected: " << result << "\nGot: " << nresult << endl;
		}
		else
			ok++;
	}
	cout << "Fine: " << ok << "/" << N_TESTS << " riusciti (" << std::setprecision(5) << 100. * (double)ok/(double)N_TESTS << "\%)\n";
	double time = double(clock() - start) / CLOCKS_PER_SEC;
	double timePerOperation = time / (double)N_TESTS;
	cout << "Tempo impiegato: " << time << " s\n";
	cout << "Tempo medio per operazione: " << setprecision(18) << timePerOperation << " s\n\n";
}

template<template<typename T, typename K> class functor>
void test2(const long long N_TESTS, char *msg) {
	cout << msg << " (" << N_TESTS << " casi di test)\n";
	long long ok = 0;
	clock_t start = clock();
	for(long long i = 0; i < N_TESTS; i++) {
		long long first = rand() % 9223372036854775807LL - rand() % 9223372036854775807LL;
		long long second = rand() % 9223372036854775807LL - rand() % 9223372036854775807LL;
		bool result = functor<long long, integer>()(first, second);
		integer nfirst{to_string(first)}, nsecond{to_string(second)};
		bool nresult = functor<integer, long long>()(nfirst, nsecond);
		if(result != nresult) {
			cerr << "Error:\n";
			cerr << "first: " << first << endl << "second: " << second << endl << "nfirst: " << nfirst << endl << "nsecond: " << nsecond << endl << endl;
			cerr << "Expected: " << result << "\nGot: " << nresult << endl;
		}
		else
			ok++;
	}
	cout << "Fine: " << ok << "/" << N_TESTS << " riusciti (" << std::setprecision(5) << 100. * (double)ok/(double)N_TESTS << "\%)\n";
	double time = double(clock() - start) / CLOCKS_PER_SEC;
	double timePerOperation = time / (double)N_TESTS;
	cout << "Tempo impiegato: " << time << " s\n";
	cout << "Tempo medio per operazione: " << setprecision(18) << timePerOperation << " s\n\n";
}

int main() {

	srand(time(NULL));

	const long long N_TESTS = 10e5;
	test<Sub>(N_TESTS, "Inizio il test per la sottrazione");
	test<Add>(N_TESTS, "Inizio il test per la addizione");
	test2<LEQ>(N_TESTS, "Inizio il test per <=");
	test2<GEQ>(N_TESTS, "Inizio il test per >=");
	test2<EQU>(N_TESTS, "Inizio il test per ==");
	test2<NEQ>(N_TESTS, "Inizio il test per !=");
	test2<Less>(N_TESTS, "Inizio il test per <");
	test2<Greater>(N_TESTS, "Inizio il test per >");

	return 0;
}
