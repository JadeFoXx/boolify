#pragma once
#include <vector>
#include <string>
#include "Term.h"
#include "RecursiveDecentParser.h"

using std::string;
using std::vector;

class Truthtable
{
public:
	Truthtable(Term, RecursiveDecentParser);
	~Truthtable();
	vector<vector<int>> getTable() const;
	string getDNF();
	string getKNF();
private:
	Term term;
	vector<vector<int>> table;
	string dnf;
	string knf;
	vector<vector<char>> minTermBits;
	void generate(int n);
	void solve(RecursiveDecentParser);
	void buildDNF();
	void buildKNF();
	void minify(vector<vector<char>>);
};

