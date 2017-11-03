#pragma once
#include <string>
#include <vector>
using std::vector;
using std::string;

class Term
{
public:
	Term();
	~Term();
private:
	string *expression;
	vector<char> *literals;
};

