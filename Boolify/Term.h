#pragma once
#include <string>
#include <vector>
#include <map>

using std::vector;
using std::string;
using std::map;
class Term
{
public:
	Term(string expression);
	~Term();
	string getExpression();
	vector<char> getLiterals();
	int charCount();
	int litCount();
private:
	string expression;
	vector<char> *literals;
	int cCount;
	int lCount;
};

