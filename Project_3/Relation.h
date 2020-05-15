#ifndef REL_H
#define REL_H

#include "Tuple.h"
#include "Predicate.h"
#include <set>
using namespace std;

class Relation : public set<vector<string>>
{
public:
	Relation() {};
	Relation(Predicate scheme) :
		scheme(scheme),
		nameStr(scheme.id())
	{
		vector<Parameter> params = scheme.getParamList();
		for (size_t i = 0; i < params.size(); i++) {
			attributes.push_back(params[i].value());
		}
	};

	string name() {
		return nameStr;
	}

	vector<string> attrList() {
		return attributes;
	}

private:
	Predicate scheme;
	string nameStr;
	vector<string> attributes;
};

#endif