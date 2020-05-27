#ifndef REL_H
#define REL_H

#include "Tuple.h"
#include "Header.h"
#include <set>
using namespace std;

class Relation : public set<Tuple>
{
public:
	Relation() {};
	Relation(string name, vector<string> headStrings) :
		nameStr(name)
	{
		for (size_t i = 0; i < headStrings.size(); i++) {
			header.push_back(headStrings[i]);
		}
	};
	string name() {
		return nameStr;
	}
	Header getHeader() {
		return header;
	}



private:
	string nameStr;
	Header header;
};

#endif