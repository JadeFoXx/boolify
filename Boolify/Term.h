#pragma onc
#include <vector>
#include <string>

class Term
{
public:
	Term(char);
	~Term();
	char getLiteral();
	bool isNegated();
	void negate();
	void addChild(Term);
	std::vector<Term> getChildren();
};

