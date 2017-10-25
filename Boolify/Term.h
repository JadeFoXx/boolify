#pragma once
#include <vector>
#include <string>

class Term
{
public:
	Term(char);
	Term(const Term &obj);
	~Term();
	char getLiteral();
	bool isNegated();
	void negate();
	void addChild(Term);
	std::vector<Term> getChildren();

private:
	char *lit;
	bool *negated;

};

