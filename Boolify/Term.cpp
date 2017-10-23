#include "stdafx.h"
#include "Term.h"


char literal;
bool negated;
std::vector<Term> children;

Term::Term(char lit)
{
	literal = lit;
}


Term::~Term()
{
}

char Term::getLiteral() {
	return literal;
}

bool Term::isNegated() {
	return negated;
}

void Term::negate() {
	negated = true;
}

void Term::addChild(Term t) {
	children.push_back(t);
}

std::vector<Term> Term::getChildren() {
	return children;
}
