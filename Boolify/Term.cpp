#include "stdafx.h"
#include "Term.h"


std::vector<Term> children;

Term::Term(char name)
{
	lit = new char;
	*lit = name;
}

Term::Term(const Term &obj) {
	lit = new char;
	*lit = *obj.lit;
}


Term::~Term()
{
}

char Term::getLiteral() {
	return *lit;
}

bool Term::isNegated() {
	return negated;
}

void Term::negate() {
	negated = new bool;
	*negated = true;
}

void Term::addChild(Term t) {
	children.push_back(t);
}

std::vector<Term> Term::getChildren() {
	return children;
}
