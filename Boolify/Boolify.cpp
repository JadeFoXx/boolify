// Boolify.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include "Term.h"


int main()
{
	std::string input;
	std::cout << "Enter your boolean term: " << std::endl;
	std::getline(std::cin, input);
	std::vector<Term*> terms;
	for (unsigned i = 0; i < input.size(); i++) {
		Term* t = new Term(input[i]);
		terms.emplace_back(t);
	}
	for (unsigned i = 0; i < terms.size(); i++) {
		std::cout << terms.at(i)->getLiteral() << std::endl;
	}
}

