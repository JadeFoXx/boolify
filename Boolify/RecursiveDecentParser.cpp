#include "stdafx.h"
#include "RecursiveDecentParser.h"

//////////////////////////////////////////////////
RecursiveDecentParser::RecursiveDecentParser()
{
}

//////////////////////////////////////////////////
RecursiveDecentParser::~RecursiveDecentParser()
{
}

//////////////////////////////////////////////////
bool RecursiveDecentParser::isNumber(char c) {
	return c >= '0' && c <= '9';
}

//////////////////////////////////////////////////
int RecursiveDecentParser::negate(int v) {
	return abs(v - 1);
}

//////////////////////////////////////////////////
int RecursiveDecentParser::or (int a, int b) {
	if ((a + b) >= 1) {
		return 1;
	}
	return 0;
}

//////////////////////////////////////////////////
int RecursiveDecentParser::and(int a, int b) {
	return a*b;
}

//////////////////////////////////////////////////
char RecursiveDecentParser::peek() {
	if (index < expression->size()) {
		return (*expression)[index];
	}
	return false;
}

//////////////////////////////////////////////////
char RecursiveDecentParser::next() {
	char c = (*expression)[index];
	index++;
	return c;
}

//////////////////////////////////////////////////
int RecursiveDecentParser::number() {
	int result = next() -'0';
	while (peek() && isNumber(peek())) {
		result = 10 * result + next() - '0';
	}
	return result;
}

//////////////////////////////////////////////////
int RecursiveDecentParser::factor() {
	if (isNumber(peek())) {
		return number();
	}
	else if (peek() == '(') {
		next();
		int result = term();
		next();
		return result;
	}
	else if (peek() == '!') {
		next();
		return negate(factor());
	}
	return 0;
}

//////////////////////////////////////////////////
int RecursiveDecentParser::term() {
	int result = factor();
	while (peek() == '*' || peek() == '+') {
		if (peek() == '*') {
			next();
			result = and (result, factor());
		}
		else if (peek() == '+') {
			next();
			result = or (result, factor());
		}
	}
	return result;
}

//////////////////////////////////////////////////
int RecursiveDecentParser::evaluate(string exp) {
	delete expression;
	index = 0;
	expression = new vector<char>;
	for (unsigned i = 0; i < exp.length(); i++) {
		expression->push_back(exp[i]);
	}
	return term();
}
