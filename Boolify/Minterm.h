#pragma once
#include <vector>
using std::vector;
class Minterm
{
public:
	Minterm(vector<int>, vector<char>);
	~Minterm();
	vector<int> getIndices() const;
	vector<char> getBits() const;
	void setCombined(bool);
	bool wasCombined() const;
	int literalCount() const;
	friend bool operator == (const Minterm& ma, const Minterm& mb);
private:
	bool combined;
	vector<int> indices;
	vector<char> bits;
};

