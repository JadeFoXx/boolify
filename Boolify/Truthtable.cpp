#include "stdafx.h"
#include "Truthtable.h"
#include <algorithm>
#include "RecursiveDecentParser.h"

//////////////////////////////////////////////////
Truthtable::Truthtable(Term t, RecursiveDecentParser rdp) : term(t), dnf(""), knf("")
{
	generate(term.litCount());
	solve(rdp);
	buildDNF();
	buildKNF();
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
void Truthtable::solve(RecursiveDecentParser rdp) {
	for (unsigned i = 0; i < (1 << term.litCount()); ++i) {
		string tmp = term.getExpression();
		for (unsigned j = 0; j < term.litCount(); ++j) {
			char lit = term.getLiterals()[j];
			char val = '0' + table[j][i];
			std::replace(tmp.begin(), tmp.end(), lit, val);
		}
		int x = rdp.evaluate(tmp);
		table[term.litCount()][i] = x;
	}
}

//////////////////////////////////////////////////
void Truthtable::buildDNF() {
	for (unsigned i = 0; i < (1 << term.litCount()); i++) {
		if (table[term.litCount()][i] == 1) {
			vector<char> bits;
			dnf += '(';
			for (unsigned j = 0; j < term.litCount(); j++) {
				if (table[j][i] == 1) {
					dnf += term.getLiterals()[j];
					bits.push_back('1');
				}
				else {
					dnf += '!';
					dnf += term.getLiterals()[j];
					bits.push_back('0');
				}
				if (j < term.getLiterals().size() - 1) {
					dnf += '*';
				}
			}
			minTermBits.push_back(bits);
			dnf += ')';
			dnf += '+';
		}
	}
	dnf = dnf.substr(0, dnf.size() - 1);
}

//////////////////////////////////////////////////
void Truthtable::buildKNF() {
	for (unsigned i = 0; i < (1 << term.litCount()); i++) {
		if (table[term.litCount()][i] == 0) {
			knf += '(';
			for (unsigned j = 0; j < term.litCount(); j++) {
				if (table[j][i] == 0) {
					knf += term.getLiterals()[j];
				}
				else {
					knf += '!';
					knf += term.getLiterals()[j];
				}
				if (j < term.litCount() - 1) {
					knf += '+';
				}
			}
			knf += ')';
			knf += '*';
		}
	}
	knf = knf.substr(0, knf.size() - 1);
}

//////////////////////////////////////////////////
string Truthtable::getDNF() {
	return dnf;
}

//////////////////////////////////////////////////
string Truthtable::getKNF() {
	return knf;
}

//////////////////////////////////////////////////
void Truthtable::minify(vector<vector<char>> bits) {
	map<vector<char>, vector<char>> implicants;
	bool didWork = false;
	for (unsigned i = 0; i < bits.size(); i++) {
		if (bits[i][0] != '*') {
			bool couldCombine = false;
			for (unsigned j = i; j < bits.size(); j++) {
				if (j != i) {
					unsigned diff = 0;
					unsigned diffIndex = 0;
					for (unsigned k = 0; k < bits[i].size(); k++) {
						if (bits[i][k] != bits[j][k]) {
							diff++;
							diffIndex = k;
						}
					}
					if (diff == 1) {
						couldCombine = true;
						vector<char> implicant;
						for (unsigned k = 0; k < bits[i].size(); k++) {
							if (k == diffIndex) {
								implicant.push_back('-');
							}
							else {
								implicant.push_back(bits[i][k]);
							}
						}
						implicants.push_back(implicant);
						didWork = true;
					}
				}
			}
			if (!couldCombine) {
				bits[i].insert(bits[i].begin(), '*');
				implicants.push_back(bits[i]);
				didWork = true;
			}
		}
	}
	if (didWork) {
		minify(implicants);
	}
}