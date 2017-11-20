#include "stdafx.h"
#include "Truthtable.h"
#include <algorithm>
#include "RecursiveDecentParser.h"
#include <math.h>

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
	for (unsigned i = 0; i < minTerms.size() - 1; i++) {
		for (unsigned j = i + 1; j < minTerms.size(); j++) {
			vector<int> cIndex = joinVectorNoDupes(minTerms[i].getIndices(), minTerms[j].getIndices());
			vector<char> cBits;
			unsigned diff = 0;
			for (unsigned k = 0; k < minTerms[i].getBits().size(); k++) {
				if (minTerms[i].getBits()[k] != minTerms[j].getBits()[k]) {
					diff++;
					cBits.push_back('-');
				}
				else {
					cBits.push_back(minTerms[i].getBits()[k]);
				}
			}
			if (diff == 1) {
				minTerms[i].setCombined(true);
				minTerms[j].setCombined(true);
				Minterm m(cIndex, cBits);
				if (!contains(primeImplicants, m)) {
					primeImplicants.push_back(m);
				}
			}
		}
	}
	unsigned noCombineCount = 0;
	for (unsigned i = 0; i < minTerms.size(); i++) {
		if (!minTerms[i].wasCombined()) {
			noCombineCount++;
			primeImplicants.push_back(minTerms[i]);
		}
	}
	if (noCombineCount == minTerms.size()) {
		return primeImplicants;
	}
	else {
		return getPrimeImplicants(primeImplicants);
	}
}


string Truthtable::getSimplifiedTerm(vector<int> indices, vector<Minterm> primeImplicants) {
	vector<vector<vector<int>>> sums;
	for (unsigned i = 0; i < indices.size(); i++) {
		vector<vector<int>> sum;
		for (unsigned j = 0; j < primeImplicants.size(); j++) {
			if (contains(primeImplicants[j].getIndices(), indices[i])) {
				vector<int> product;
				product.push_back(j);
				sum.push_back(product);
			}
		}
		sums.push_back(sum);
	}

}



vector<vector<vector<int>>> Truthtable::multiply(vector<vector<vector<int>>> prev) {
	vector<vector<vector<int>>> next;
	if (prev.size() == 1) {
		return prev;
	}
	else {
		vector<vector<int>> a = prev[0];
		vector<vector<int>> b = prev[1];
		vector<vector<int>> c;
		for (vector<int> v1 : a) {
			for (vector<int> v2 : b) {
				c.push_back(joinVectorNoDupes(v1, v2));
			}
		}
		next.push_back(c);
		for (unsigned i = 2; i < prev.size(); i++) {
			next.push_back(prev[i]);
		}
		return multiply(next);
	}
}



vector<vector<int>> Truthtable::absorb(vector<vector<int>> prev) {
	vector<vector<int>> minimas;
	int minSize = prev[0].size();
	for (unsigned i = 1; i < prev.size(); i++) {
		if (prev[i].size() < minSize) {
			minSize = prev[i].size();
		}
	}
	for (unsigned i = 0; i < prev.size(); i++) {
		if (prev[i].size() == minSize) {
			minimas.push_back(prev[i]);
		}
	}

}
