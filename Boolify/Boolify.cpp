// Boolify.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <cctype>

using std::vector;
using std::cout;
using std::string;



const char *expr;

bool isNumber(char c) {
	return c >= '0' && c <= '9';
}

int negate(int v) {
	return abs(v - 1);
}

int or (int a, int b) {
	if ((a + b) >= 1) {
		return 1;
	}
	return 0;
}

int and(int a, int b) {
	return a*b;
}

char peek() {
	return *expr;
}

char get() {
	return *expr++;
}

int number() {
	int result = get() - '0';
	while (isNumber(peek())) {
		result = 10 * result + get() - '0';
	}
	return result;
}

int term();

int factor() {
	if (isNumber(peek())) {
		return number();
	}
	else if(peek() == '(') {
		get();
		int result = term();
		get();
		return result;
	}
	else if (get() == '!') {
		return negate(factor());
	}
	return 0;
}

int term() {
	int result = factor();
	while (peek() == '*' || peek() == '+') {
		if (get() == '*') {
			result = and (result, factor());
		}
		else if(get() == '+') {
			result = or (result, factor());
		}
	}
	return result;
}

int eval(string expression) {
	expr = expression.c_str();
	return term();
}



int main()
{
	std::string input;
	while (input != "exit") {


		cout << "Enter your boolean term: " << std::endl;
		std::getline(std::cin, input);
		vector<char> literals;

		for (unsigned i = 0; i < input.size(); i++) {
			if (!(std::find(literals.begin(), literals.end(), input[i]) != literals.end())) {
				if (isalpha(input[i])) {
					literals.push_back(input[i]);
				}
			}
		}
		cout << "Generating truth table..." << std::endl;
		const unsigned n = literals.size();
		vector<vector<int>> truthTable(n, vector<int>(1 << n));
		vector<int> p;
		unsigned fill = 1 << (n - 1);
		for (unsigned col = 0; col < n; ++col, fill >>= 1) {
			for (unsigned row = fill; row < (1 << n); row += (fill * 2)) {
				std::fill_n(&truthTable[col][row], fill, 1);
			}
		}

		for (unsigned i = 0; i < (1 << n); ++i) {
			string tmp = input;
			for (unsigned j = 0; j < n; ++j) {
				char lit = literals[j];
				char val = '0' + truthTable[j][i];
				std::replace(tmp.begin(), tmp.end(), lit, val);
			}
			p.push_back(eval(tmp));
		}


		for (unsigned i = 0; i < n; ++i) {
			cout << literals[i] << " ";
		}
		cout << "p";
		cout << std::endl;
		for (unsigned i = 0; i < (1 << n); ++i) {
			for (unsigned j = 0; j < n; ++j) {
				cout << truthTable[j][i] << " ";
			}
			cout << p[i];
			cout << std::endl;
		}
	}
}

