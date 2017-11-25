#pragma once
#include <vector>
#include <string>
#include "Term.h"
#include "Minterm.h"
#include "VectorUtils.h"
#include "LogicEngine.h"

using std::string;
using std::vector;

class Truthtable
{
public:
	Truthtable(Term, LogicEngine);
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
	string simplifiedTerm;
	void generate(int);
	void solve(LogicEngine);
};

