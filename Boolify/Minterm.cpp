#include "stdafx.h"
#include "Minterm.h"


Minterm::Minterm(vector<int> index, vector<char> bits) : index(index), bit(bits), prime(false)
{
}


Minterm::~Minterm()
{
}

vector<int> Minterm::getIndex() const {
	return index;
}

vector<char> Minterm::getBit() const {
	return bit;
}

void Minterm::setPrime(bool b) {
	prime = b;
}

bool Minterm::isPrime() const {
	return prime;
}

void Minterm::setCombined(bool b) {
	combined = b;
}

bool Minterm::wasCombined() const {
	return combined;
}

bool Minterm::equals(Minterm m) {
	bool equal = true;
	for (unsigned i = 0; i < bit.size(); i++) {
		if (bit[i] != m.bit[i]) {
			equal = false;
		}
	}
	return equal;
}