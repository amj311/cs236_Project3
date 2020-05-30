#ifndef REL_H
#define REL_H

#include "Tuple.h"
#include "Header.h"
#include <algorithm>
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
			header.scheme.push_back(headStrings[i]);
		}
	};
	string name() {
		return nameStr;
	}
	Header getHeader() {
		return header;
	}

	Relation select(size_t col, string toMatch) {
		Relation newRel(nameStr, header.scheme);
		for (Relation::iterator it = this->begin(); it != this->end(); it++) {
			if ((*it)[col] == toMatch) newRel.insert((*it));
		}

		return newRel;
	}

	Relation selectIfEqual(vector<size_t> cols) {
		Relation newRel(nameStr, header.scheme);
		for (Relation::iterator it = this->begin(); it != this->end(); it++) {
			bool allMatch = true;
			for (size_t i = 0; i < cols.size(); i++) {
				if ((*it)[cols[i]] != (*it)[cols[0]]) allMatch = false;
			}
			if (allMatch) newRel.insert((*it));
		}

		return newRel;
	}

	Relation project(vector<size_t> cols) {
		vector<string> newScheme;
		for (size_t i = 0; i < cols.size(); i++) {
			newScheme.push_back(header.scheme[cols[i]]);
		}

		Relation newRel(nameStr, newScheme);

		for (Relation::iterator it = this->begin(); it != this->end(); it++) {
			Tuple newTuple = Tuple();
			for (size_t i = 0; i < cols.size(); i++) {
				newTuple.push_back((*it)[cols[i]]);
			}
			newRel.insert(newTuple);
		}

		return newRel;
	}

	Relation rename(size_t col, string newName) {
		vector<string> newScheme = header.scheme;
		newScheme[col] = newName;

		Relation newRel(nameStr, newScheme);

		for (Relation::iterator it = this->begin(); it != this->end(); it++) {
			newRel.insert(*it);
		}

		return newRel;
	}

private:
	string nameStr;
	Header header;
};

#endif