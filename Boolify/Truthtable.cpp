#include "stdafx.h"
#include "Truthtable.h"


Truthtable::Truthtable(int n)
{
	generate(n);
}

Truthtable::~Truthtable()
{
	delete expression;
	delete literals;
	delete table;
	delete p;
}

string Truthtable::getExpression() const {
	return *expression;
}

vector<char> Truthtable::getLiterals() const {
	return *literals;
}

vector<vector<int>> Truthtable::getTable() const {
	return *table;
}

vector<int> Truthtable::getP() const {
	return *p;
}

void Truthtable::extractLiterals(string expression) {
	literals = new vector<char>;
	for (unsigned i = 0; i < expression.size(); i++) {
		if (isalpha(expression[i])) {
			literals->push_back(expression[i]);
		}
	}
}

void Truthtable::generate(const int n) {
	table = new vector<vector<int>>(n, vector<int>(1 << n));
	p = new vector<int> (1 << n);
	unsigned fill = 1 << (n - 1);
	for (unsigned col = 0; col < n; col++, fill >>= 1) {
		for (unsigned row = fill; row < (1 << n); row += (fill * 2)) {
			std::fill_n(&table[col][row], fill, 1);
		}
	}
}
