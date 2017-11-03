#pragma once
#include <vector>
#include <string>
using std::string;
using std::vector;
class Truthtable
{
public:
	Truthtable(int n);
	~Truthtable();
	string getExpression() const;
	vector<char> getLiterals() const;
	vector<vector<int>> getTable() const;
	vector<int> getP() const;
private:
	string *expression;
	vector<char> *literals;
	vector<vector<int>> *table;
	vector<int> *p;

	void extractLiterals(string expression);
	void generate(int n);
};

