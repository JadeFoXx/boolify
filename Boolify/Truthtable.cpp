#include "stdafx.h"
#include "Truthtable.h"

Truthtable::Truthtable(int n)
{
	generate(n);
}

Truthtable::~Truthtable()
{
}

vector<vector<int>> Truthtable::getTable() const {
	return table;
}

vector<int> Truthtable::getP() const {
	return p;
}

void Truthtable::generate(const int n) {
	table = vector<vector<int>>(n, vector<int>(1 << n));
	p = vector<int> (1 << n);
	unsigned number_to_fill = 1U << (n - 1);
	for (unsigned col = 0; col < n; col++, number_to_fill >>= 1U) {
		for (unsigned row = number_to_fill; row < (1U << n); row += (number_to_fill * 2)) {
			std::fill_n(&table[col][row], number_to_fill, 1);
		}
	}
}
