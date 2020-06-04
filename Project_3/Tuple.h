#ifndef TUPLE_H
#define TUPLE_H

#include <vector>
#include "Parameter.h"
using namespace std;

class Tuple : public vector<string>
{
public:
	Tuple() {};
	Tuple(vector<Parameter> params) {
		for (size_t i = 0; i < params.size(); i++) {
			this->push_back(params[i].value());
		}
	};

	bool canJoinWith(Tuple otherTuple, vector<pair<size_t, size_t>> colPairs) {
		bool canJoin = true;
		vector<size_t> usedIdxs;
		for (size_t i = 0; i < colPairs.size(); i++) {
			if ((*this)[colPairs[i].first] != otherTuple[colPairs[i].second]) {
				canJoin = false;
				break;
			}
			else usedIdxs.push_back(colPairs[i].second);
		}
		return canJoin;
	}
};

#endif