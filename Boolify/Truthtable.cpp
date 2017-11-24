#include "stdafx.h"
#include "Truthtable.h"
#include <algorithm>
#include "RecursiveDecentParser.h"
#include <math.h>

//////////////////////////////////////////////////
Truthtable::Truthtable(Term t, RecursiveDecentParser rdp) : term(t), dnf(""), knf(""), simplifiedTerm("")
{
	generate(term.litCount());
	solve(rdp);
	buildDNF();
	buildKNF();
	quineMcClusky();
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
			minTerms.push_back(m);
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
void Truthtable::quineMcClusky() {
	vector<Minterm> primeImplicants = getPrimeImplicants(minTerms);
	vector<int> primeIndices = getIndices(minTerms);
	vector<vector<vector<int>>> primeSums = sum(primeIndices, primeImplicants);
	vector<vector<vector<int>>> primeProducts = multiply(primeSums);
	vector<vector<int>> result = absorb(primeProducts[0]);
	simplifiedTerm = concatinate(result, primeImplicants);
}

vector<vector<vector<int>>> Truthtable::sum(vector<int> indices, vector<Minterm> mTerms) {
	vector<vector<vector<int>>> sums;
	for (unsigned i = 0; i < indices.size(); i++) {
		vector<vector<int>> sum;
		for (unsigned j = 0; j < mTerms.size(); j++) {
			if (contains(mTerms[j].getIndices(), indices[i])) {
				vector<int> product;
				product.push_back(j);
				sum.push_back(product);
			}
		}
		sums.push_back(sum);
	}
	return sums;
}

vector<int> Truthtable::getIndices(vector<Minterm> mTerms) {
	vector<int> indices;
	for (unsigned i = 0; i < mTerms.size(); i++) {
		indices.push_back(mTerms[i].getIndices()[0]);
	}
	return indices;
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
	unsigned notCombineCount = 0;
	for (unsigned i = 0; i < minTerms.size(); i++) {
		if (!minTerms[i].wasCombined()) {
			notCombineCount++;
			primeImplicants.push_back(minTerms[i]);
		}
	}
	if (notCombineCount == minTerms.size()) {
		return primeImplicants;
	}
	else {
		return getPrimeImplicants(primeImplicants);
	}
}

//////////////////////////////////////////////////
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


//////////////////////////////////////////////////
vector<vector<int>> Truthtable::absorb(vector<vector<int>> prev) {
	int minSize = prev[0].size();
	vector<int> combined;
	vector<vector<int>> minimas;
	for (unsigned i = 0; i < prev.size(); i++) {
		for (unsigned j = i+1; j < prev.size(); j++) {
			bool includes = true;
			if (prev[i].size() <= prev[j].size()) {
				for (unsigned k = 0; k < prev[i].size(); k++) {
					if (!contains(prev[j], prev[i][k])) {
						includes = false;
					}
				}
				if (includes) {
					combined.push_back(j);
				}
			}
			else {
				for (unsigned k = 0; k < prev[j].size(); k++) {
					if (!contains(prev[i], prev[j][k])) {
						includes = false;
					}
				}
				if (includes) {
					combined.push_back(i);
				}
			}
			
		}
	}
	for (unsigned i = 0; i < prev.size(); i++) {
		if (prev[i].size() < minSize) {
			minSize = prev[i].size();
		}
	}
	for (int i = 0; i < prev.size(); i++) {
		if (!contains(combined, i) && prev[i].size() == minSize) {
			minimas.push_back(prev[i]);
		}
	}
	return minimas;
}

//////////////////////////////////////////////////
string Truthtable::concatinate(vector<vector<int>> products, vector<Minterm> primeImplicants) {
	string concatinated = "";
	int min = 0;
	for (int i : products[0]) {
		min += primeImplicants[i].literalCount();
	}
	int index = 0;
	for (unsigned k = 0; k < products.size(); k++) {
		int m = 0;
		for (int i : products[k]) {
			m += primeImplicants[i].literalCount();
		}
		if (m < min) {
			min = m;
			index = k;
		}
	}
	bool firstSum = true;
	for (int i : products[index]) {
		if (!firstSum) {
			concatinated += '+';
		}
		concatinated += '(';
		vector<char> c = primeImplicants[i].getBits();
		bool firstProduct = true;
		for (unsigned k = 0; k < c.size(); k++) {
			if (c[k] != '-') {
				if (!firstProduct) {
					concatinated += '*';
				}
				if (c[k] == '0') {
					concatinated += '!';
					concatinated += term.getLiterals()[k];
				}
				else {
					concatinated += term.getLiterals()[k];
				}
				firstProduct = false;
			}
		}
		concatinated += ')';
		firstSum = false;
	}
	return concatinated;
}

//////////////////////////////////////////////////
string Truthtable::getSimpleTerm() const {
	return simplifiedTerm;
}
