#include "stdafx.h"
#include "Truthtable.h"
#include <algorithm>
#include <math.h>

//////////////////////////////////////////////////
Truthtable::Truthtable(Term t, LogicEngine lgce) : term(t), dnf(""), knf(""), simplifiedTerm("")
{
	generate(term.litCount());
	solve(lgce);
	dnf = lgce.buildDNF(table, t);
	knf = lgce.buildKNF(table, t);
	simplifiedTerm = lgce.quineMcClusky(t, lgce.getMinterms(table, t));
}

//////////////////////////////////////////////////
Truthtable::~Truthtable()
{
}

//////////////////////////////////////////////////
vector<vector<int>> Truthtable::getTable() const {
	return table;
}

//////////////////////////////////////////////////
void Truthtable::generate(const int n) {
	table = vector<vector<int>>(n+1, vector<int>(1 << n));
	unsigned number_to_fill = 1U << (n - 1);
	for (unsigned col = 0; col < n; col++, number_to_fill >>= 1U) {
		for (unsigned row = number_to_fill; row < (1U << n); row += (number_to_fill * 2)) {
			std::fill_n(&table[col][row], number_to_fill, 1);
		}
	}
}

//////////////////////////////////////////////////
void Truthtable::solve(LogicEngine lgce) {
	for (unsigned i = 0; i < (1 << term.litCount()); ++i) {
		string tmp = term.getExpression();
		for (unsigned j = 0; j < term.litCount(); ++j) {
			char lit = term.getLiterals()[j];
			char val = '0' + table[j][i];
			std::replace(tmp.begin(), tmp.end(), lit, val);
		}
		int x = lgce.evaluate(tmp);
		table[term.litCount()][i] = x;
	}
}


//////////////////////////////////////////////////
string Truthtable::getDNF() const {
	return dnf;
}

//////////////////////////////////////////////////
string Truthtable::getKNF() const {
	return knf;
}

//////////////////////////////////////////////////
string Truthtable::getSimpleTerm() const {
	return simplifiedTerm;
}
