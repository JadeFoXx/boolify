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
	vector<vector<int>> getTable() const;
	vector<int> getP() const;
private:
	vector<vector<int>> table;
	vector<int> p;
	void generate(int n);
};

