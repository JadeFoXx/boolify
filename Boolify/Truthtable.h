#pragma once
#include <vector>
#include <string>
#include "Term.h"
#include "RecursiveDecentParser.h"
#include "Minterm.h"
#include "VectorUtils.h"

using std::string;
using std::vector;

class Truthtable
{
public:
	Truthtable(Term, RecursiveDecentParser);
	~Truthtable();
	vector<vector<int>> getTable() const;
	string getDNF() const;
	string getKNF() const;
	string getSimpleTerm() const;
private:
	Term term;
	vector<vector<int>> table;
	string dnf;
	string knf;
	vector<Minterm> minTerms;
	string simplifiedTerm;
	void generate(int);
	void solve(RecursiveDecentParser);
	void buildDNF();
	void buildKNF();
	void quineMcClusky();
	vector<int> getIndices(vector<Minterm>);
	vector<Minterm> getPrimeImplicants(vector<Minterm>);
	vector<vector<vector<int>>> sum(vector<int>, vector<Minterm>);
	vector<vector<vector<int>>> multiply(vector<vector<vector<int>>>);
	vector<vector<int>> absorb(vector<vector<int>>);
	string concatinate(vector<vector<int>>, vector<Minterm>);
};

