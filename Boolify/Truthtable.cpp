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
	vector<Minterm> pI = getPrimeImplicants(startTerms);
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
	for (int i = 0; i < (1 << term.litCount()); i++) {
		if (table[term.litCount()][i] == 1) {
			vector<char> bits;
			vector<int> index;
			index.push_back(i);
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
			Minterm m(index, bits);
			startTerms.push_back(m);
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
string Truthtable::getDNF() const {
	return dnf;
}

//////////////////////////////////////////////////
string Truthtable::getKNF() const {
	return knf;
}

//////////////////////////////////////////////////
vector<Minterm> Truthtable::getPrimeImplicants(vector<Minterm> minTerms) {
	vector<Minterm> primeImplicants;
	bool didWork = false;
	for (unsigned i = 0; i < minTerms.size(); i++) {
		for (unsigned j = 0; j < minTerms.size(); j++) {
			vector<int> implicantIndex;
			vector<char> implicantBit;
			if (j != i) {
				bool prime = true;
				unsigned diff = 0;
				implicantIndex = joinVectorNoDupes(minTerms[i].getIndex(), minTerms[j].getIndex());
				for (unsigned k = 0; k < minTerms[i].getBit().size(); k++) {
					if (minTerms[i].getBit()[k] != minTerms[j].getBit()[k]) {
						diff++;
						implicantBit.push_back('-');
					}
					else {
						implicantBit.push_back(minTerms[i].getBit()[k]);
					}
				}
				if (diff == 1) {
					prime = false;
					Minterm mT(implicantIndex, implicantBit);
					bool dupe = false;
					for (Minterm m : primeImplicants) {
						if (m.equals(mT)) {
							dupe = true;
						}
					}
					if (!dupe) {
						primeImplicants.push_back(mT);
						didWork = true;
					}
				}
				if (prime) {
					primeImplicants.push_back(minTerms[i]);
				}
			}
		}
		
	}
	if (didWork) {
		return getPrimeImplicants(primeImplicants);
	}
	else {
		return primeImplicants;
	}
}
