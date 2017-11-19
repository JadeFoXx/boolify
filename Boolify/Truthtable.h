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
private:
	Term term;
	vector<vector<int>> table;
	string dnf;
	string knf;
	vector<Minterm> startTerms;
	void generate(int);
	void solve(RecursiveDecentParser);
	void buildDNF();
	void buildKNF();
	vector<Minterm> getPrimeImplicants(vector<Minterm>);
	string getSimplifiedTerm(vector<int>, vector<Minterm>);
	vector<vector<vector<int>>> multiply(vector<vector<vector<int>>>);
	vector<vector<int>> absorb(vector<vector<int>>);
};

