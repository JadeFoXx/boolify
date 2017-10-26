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

bool eval_expr(const char **pe, int &lhs, bool inside = false);
// gets the next char after skipping optional whitespace
char skip_ws(const char **pe) {
	while (**pe == ' ') ++(*pe);
	return **pe;
}
// evaluates a parenthesized expression or a number
bool eval_prim(const char **pe, int &res) {
	char c = skip_ws(pe);
	if (c == '(') {
		++(*pe);
		if (!eval_expr(pe, res, true)) return false;
		++(*pe);
		return true;
	}
	if (isdigit(c)) {
		res = 0;
		while (isdigit(c)) {
			res = 10 * res + c - '0';
			c = *(++(*pe));
		}
		return true;
	}
	return false;
}
// evaluates a chain of + - * / operations
bool eval_expr(const char **pe, int &lhs, bool inside) {
	if (!eval_prim(pe, lhs)) return false;
	char op;
	while ((op = skip_ws(pe)) && (op == '+' || op == '-' || op == '*' || op == '/')) {
		++(*pe);
		int rhs;
		if (!eval_prim(pe, rhs)) return false;
		switch (op) {
		case '+': lhs += rhs; break;
		case '-': lhs -= rhs; break;
		case '*': lhs *= rhs; break;
		case '/': lhs /= rhs; break;
		}
	}
	return inside ? op == ')' : !op;
}
// wrapper API to hide an extra level of indirection
bool evaluate(const char *e, int &result) {
	return eval_expr(&e, result);
}

int main()
{
	std::string input;
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

	}

	for (unsigned i = 0; i < n; ++i) {
		cout << literals[i] << " ";
	}
	cout << std::endl;
	for (unsigned i = 0; i < (1 << n); ++i) {
		for (unsigned j = 0; j < n; ++j) {
			cout << truthTable[j][i] << " ";
		}
		cout << std::endl;
	}
}

