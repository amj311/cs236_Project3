#ifndef PRED_H
#define PRED_H

#include "Parameter.h"

class Predicate
{
public:
	Predicate() {};
	Predicate(string id, vector<string> paramInput) : idStr(id) {
		for (size_t i = 0; i < paramInput.size(); i++) {
			params.push_back(Parameter(paramInput[i]));
		}
	};

	vector<Parameter> getParamList() {
		return params;
	}
	string id() {
		return idStr;
	}
	string toString() {
		string out = idStr + "(";
		for (size_t i = 0; i < params.size(); i++) {
			out += params[i].value();
			if (i < params.size() - 1) out += ",";
		}
		return out + ")";
	}

private:
	string idStr;
	vector<Parameter> params;
};

#endif