#pragma once
#include <string>
using std::string;
class RecursiveDecentParser
{
public:
	RecursiveDecentParser();
	~RecursiveDecentParser();
	int evaluate(string expr);
private:
	const char *expression;
	bool isNumber(char c);
	int negate(int i);
	int or (int a, int b);
	int and(int a, int b);
	char peek();
	char next();
	int number();
	int factor();
	int term();
};

