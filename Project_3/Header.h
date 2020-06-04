#ifndef HEADER_H
#define HEADER_H

#include <string>
#include <vector>
#include <algorithm>
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

	Header combineWithHeader(Header otherHeader) {
		vector<string> newScheme;

		for (size_t i = 0; i < scheme.size(); i++) {
			newScheme.push_back(scheme[i]);
		}

		for (size_t j = 0; j < otherHeader.scheme.size(); j++) {
			vector<string>::iterator it = find(newScheme.begin(), newScheme.end(), otherHeader.scheme[j]);
			if (it == newScheme.end()) newScheme.push_back(otherHeader.scheme[j]);
		}
		
		return Header(newScheme);
	}
};

#endif