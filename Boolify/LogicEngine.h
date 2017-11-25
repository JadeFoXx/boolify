#pragma once
#include <string>
#include <vector>
#include "Minterm.h"
#include "Term.h"
#include "VectorUtils.h"
using std::string;
using std::vector;

class LogicEngine
{
public:
	LogicEngine();
	~LogicEngine();
	// recursive decent parser 
	//////////////////////////////////////////////////
	int evaluate(string expr);

	// dnf
	//////////////////////////////////////////////////
	string buildDNF(vector<vector<int>>, Term);

	// knf
	//////////////////////////////////////////////////
	string buildKNF(vector<vector<int>>, Term);

	vector<Minterm> getMinterms(vector<vector<int>>, Term);

	// quine mcclusky
	//////////////////////////////////////////////////
	string quineMcClusky(Term, vector<Minterm>);
private:
	vector<Minterm> minterms;

	// recursive decent parser
	//////////////////////////////////////////////////
	vector<char> *expression;
	bool isNumber(char c);
	int negate(int i);
	int or (int a, int b);
	int and(int a, int b);
	char peek();
	char next();
	int number();
	int factor();
	int term();
	int index;

	// quine mcclusky
	//////////////////////////////////////////////////
	vector<int> getIndices(vector<Minterm>);
	vector<Minterm> getPrimeImplicants(vector<Minterm>);
	vector<vector<vector<int>>> sum(vector<int>, vector<Minterm>);
	vector<vector<vector<int>>> multiply(vector<vector<vector<int>>>);
	vector<vector<int>> absorb(vector<vector<int>>);
	string concatinate(Term, vector<vector<int>>, vector<Minterm>);
};

