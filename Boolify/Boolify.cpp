// Boolify.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <cctype>
#include "Term.h"
#include "Truthtable.h"

using std::vector;
using std::cout;
using std::string;


int main()
{
	std::string input;
	while (input != "exit") {

		cout << "Enter your boolean term: " << std::endl;
		std::getline(std::cin, input);
		Term term(input);
		cout << "Generating truth table..." << std::endl;
		LogicEngine lgce;
		Truthtable truthtable(term, lgce);

		for (unsigned i = 0; i < term.litCount(); ++i) {
			cout << term.getLiterals()[i] << " ";
		}
		cout << "p";
		cout << std::endl;

		for (unsigned i = 0; i < (1 << term.litCount()); ++i) {
			for (unsigned j = 0; j <= term.litCount(); ++j) {
				cout << truthtable.getTable()[j][i] << " " ;
			}
			cout << std::endl;
		}

		cout << "DNF: " << truthtable.getDNF() << std::endl;
		cout << "KNF: " << truthtable.getKNF() << std::endl;
		cout << "Simplified: " << truthtable.getSimpleTerm() << std::endl;
	}
}

