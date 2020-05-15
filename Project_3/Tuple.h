#ifndef TUPLE_H
#define TUPLE_H

#include <vector>
#include "Parameter.h"
using namespace std;

struct Tuple : public vector<string>
{
	Tuple() {};
	Tuple(vector<Parameter> params) {
		for (size_t i = 0; i < params.size(); i++) {
			this->push_back(params[i].value());
		}
	};
};

#endif