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
#include "RecursiveDecentParser.h"

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
		Truthtable truthtable(term.size());
		
		RecursiveDecentParser rdp;
		for (unsigned i = 0; i < (1 << term.size()); ++i) {
			string tmp = input;
			for (unsigned j = 0; j < term.getLiterals().size(); ++j) {
				char lit = term.getLiterals()[j];
				char val = '0' + truthtable.getTable()[j][i];
				std::replace(tmp.begin(), tmp.end(), lit, val);
			}
			truthtable.getP().push_back(rdp.evaluate(tmp));
		}


		for (unsigned i = 0; i < term.size(); ++i) {
			cout << term.getLiterals()[i] << " ";
		}
		cout << "p";
		cout << std::endl;
		for (unsigned i = 0; i < (1 << term.size()); ++i) {
			for (unsigned j = 0; j < term.size(); ++j) {
				cout << truthtable.getTable()[j][i] << " ";
			}
			cout << truthtable.getP()[i];
			cout << std::endl;
		}

		string dnf = "";
		string knf = "";
		for (unsigned i = 0; i < truthtable.getP().size(); i++) {
			if (truthtable.getP()[i] == 1) {
				dnf += '(';
				for (unsigned j = 0; j < term.size(); j++) {
					if (truthtable.getTable()[j][i] == 1) {
						dnf += term.getLiterals()[j];
					}
					else {
						dnf += '!';
						dnf += term.getLiterals()[j];
					}
					if (j < term.size()-1) {
						dnf += '*';
					}
				}
				dnf += ')';
				dnf += '+';
			}
			else {
				knf += '(';

			}
		}
		cout << dnf << std::endl;
	}
}

