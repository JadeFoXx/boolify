#pragma once

//////////////////////////////////////////////////
template <typename T> vector<T> joinVectorNoDupes(vector<T> a, vector<T> b) {
	vector<T> joined;
	for (T x : a) {
		joined.push_back(x);
	}
	for (T y : b) {
		if (std::find(joined.begin(), joined.end(), y) == joined.end()) {
			joined.push_back(y);
		}
	}
	return joined;
}

//////////////////////////////////////////////////
template <typename T> bool contains(vector<T> a, T b) {
	for (T t : a) {
		if (t == b) {
			return true;
		}
	}
	return false;
}