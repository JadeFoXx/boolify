#include "stdafx.h"
#include "Minterm.h"

//////////////////////////////////////////////////
Minterm::Minterm(vector<int> index, vector<char> bits) : indices(index), bits(bits)
{
}

//////////////////////////////////////////////////
Minterm::~Minterm()
{
}

//////////////////////////////////////////////////
vector<int> Minterm::getIndices() const {
	return indices;
}

//////////////////////////////////////////////////
vector<char> Minterm::getBits() const {
	return bits;
}

//////////////////////////////////////////////////
void Minterm::setCombined(bool b) {
	combined = b;
}

//////////////////////////////////////////////////
bool Minterm::wasCombined() const {
	return combined;
}

//////////////////////////////////////////////////
int Minterm::literalCount() const {
	int counter = 0;
	for (char c : bits) {
		if (c != '-') {
			counter++;
		}
	}
	return counter;
}

//////////////////////////////////////////////////
bool operator==(const Minterm& ma, const Minterm& mb) {
	return ma.getBits() == mb.getBits();
}

