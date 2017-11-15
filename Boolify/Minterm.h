#pragma once
#include <vector>
using std::vector;
class Minterm
{
public:
	Minterm(vector<int>, vector<char>);
	~Minterm();
	vector<int> getIndex() const;
	vector<char> getBit() const;
	void setPrime(bool);
	bool isPrime() const;
	void setCombined(bool);
	bool wasCombined() const;
	bool equals(Minterm);
private:
	bool prime;
	bool combined;
	vector<int> index;
	vector<char> bit;
};

