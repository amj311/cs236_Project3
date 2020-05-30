#ifndef HEADER_H
#define HEADER_H

#include <string>
#include <vector>
using namespace std;

struct Header
{
	Header() {};
	Header(vector<string> scheme) : scheme(scheme) {};
	vector<string> scheme;
	
	size_t indexOf(string colName) {
		size_t col = 0;
		for (; col < scheme.size(); col++) {
			if (scheme[col] == colName) return col;
		}
		return -1;
	}
};

#endif