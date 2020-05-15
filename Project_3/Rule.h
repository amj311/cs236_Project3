#ifndef RULE_H
#define RULE_H

#include "Predicate.h"
#include <string>
#include <vector>
using namespace std;

class Rule
{
public:
	Rule(Predicate headPred, vector<Predicate> predList) : headPred(headPred), preds(predList) {};

	vector<Predicate> getPredList() {
		return preds;
	}
	Predicate head() {
		return headPred;
	}
	string toString() {
		string out = headPred.toString() + " :- ";
		for (size_t i = 0; i < preds.size(); i++) {
			out += preds[i].toString();
			if (i < preds.size() - 1) out += ",";
		}
		return out + ".";
	}

private:
	Predicate headPred;
	vector<Predicate> preds;
};

#endif