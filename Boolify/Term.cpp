#include "stdafx.h"
#include "Term.h"

//////////////////////////////////////////////////
Term::Term(string exp)
{
	expression = exp;
	literals = new vector<char>;
	for (unsigned i = 0; i < exp.size(); i++) {
		if (isalpha(exp[i]) && !(std::find(literals->begin(), literals->end(), exp[i]) != literals->end())) {
			literals->push_back(exp[i]);
		}
	}
	cCount = exp.length();
	lCount = literals->size();
}

//////////////////////////////////////////////////
Term::~Term()
{
}

//////////////////////////////////////////////////
string Term::getExpression() {
	return expression;
}

//////////////////////////////////////////////////
vector<char> Term::getLiterals() {
	return *literals;
}

//////////////////////////////////////////////////
int Term::charCount() {
	return cCount;
}

//////////////////////////////////////////////////
int Term::litCount() {
	return lCount;
}