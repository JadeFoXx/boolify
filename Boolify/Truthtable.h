#pragma once
#include <vector>
#include <string>
#include "Term.h"
#include "RecursiveDecentParser.h"
#include "Minterm.h"

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

	template <typename T> vector<T> joinVectorNoDupes(vector<T> a, vector<T> b) {
		vector<T> joined;
		for (T x : a) {
			joined.push_back(x);
		}
		for (T y : b) {
			if (std::find(joined.begin(), joined.end(), y) == joined.end()) {
				joined.push_back(y);
			}
		}
		return joined;
	}
};

