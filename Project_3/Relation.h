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

	Relation join(Relation otherRel) {

		//determine which columns overlap
		vector<pair<size_t, size_t>> colPairs;
		for (size_t i = 0; i < header.scheme.size(); i++) {
			for (size_t j = 0; j < otherRel.getHeader().scheme.size(); j++) {
				if (header.scheme[i] == otherRel.getHeader().scheme[j]) colPairs.push_back(pair<size_t, size_t>(i, j));
			}
		}

		Header newHeader = header.combineWithHeader(otherRel.getHeader());
		Relation newRel(nameStr, newHeader.scheme);

		for (Relation::iterator ti = this->begin(); ti != this->end(); ti++) {
			for (Relation::iterator tj = otherRel.begin(); tj != otherRel.end(); tj++) {
				bool canJoin = true;
				vector<size_t> usedIdxs;
				for (size_t i = 0; i < colPairs.size(); i++) {
					if ((*ti)[colPairs[i].first] != (*tj)[colPairs[i].second]) {
						canJoin = false;
						break;
					}
					else usedIdxs.push_back(colPairs[i].second);
				}
				if (canJoin) {
					Tuple newTup = Tuple();
					for (size_t i = 0; i < (*ti).size(); i++) {
						newTup.push_back((*ti)[i]);
					}
					for (size_t j = 0; j < (*tj).size(); j++) {
						bool skipCol = false;
						for (size_t k = 0; k < usedIdxs.size() && !skipCol; k++) {
							if (j == usedIdxs[k]) skipCol = true;
						}
						if (!skipCol) newTup.push_back((*tj)[j]);
					}
					newRel.insert(newTup);
				}
			}
		}

		return newRel;
	}

	bool addFrom(Relation otherRel) {

		//determine compatibility
		for (size_t i = 0; i < header.scheme.size(); i++) {
			if (header.scheme[i] != otherRel.getHeader().scheme[i]) return false;
		}

		bool addedNew = false;
		for (Relation::iterator tj = otherRel.begin(); tj != otherRel.end(); tj++) {
			if (this->insert((*tj)).second) {
				addedNew = true;
				cout << "  ";
				for (size_t h = 0; h < header.scheme.size(); h++) {
					cout << header.scheme[h] << "=" << (*tj)[h];
					if (h < header.scheme.size() - 1) cout << ", ";
				}
				cout << endl;
			}
		}

		return addedNew;
	}



	string toString() {
		string out = "";
		for (Relation::iterator it = this->begin(); it != this->end(); it++) {
			out += "  ";
			for (size_t h = 0; h < header.scheme.size(); h++) {
				out += header.scheme[h] + "=" + (*it)[h];
				if (h < header.scheme.size() - 1) out += ", ";
			}
			out += "\n";
		}
		return out;
	}

private:
	string nameStr;
	Header header;
};

#endif