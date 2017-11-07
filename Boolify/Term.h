#pragma once
#include <string>
#include <vector>
using std::vector;
using std::string;

class Term
{
public:
	Term(string expression);
	~Term();
	string getExpression();
	vector<char> getLiterals();
	int size();
private:
	string expression;
	vector<char> *literals;
};

