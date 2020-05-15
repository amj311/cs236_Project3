#ifndef DB_H
#define DB_H

#include "Relation.h"
#include <map>

class Database
{
public:
	Database() {};

	void addRelationFromScheme(Predicate scheme) {
		relations[scheme.id()] = Relation(scheme);
	}

	bool saveFact(Predicate fact) {
		if (hasRelation(fact.id())) {
			relations[fact.id()].insert(Tuple(fact.getParamList()));
			return true;
		}
		return false;
	}

	bool hasRelation(string name) {
		map<string, Relation>::iterator it = relations.find(name);
		if (it != relations.end())
		{
			//element found;
			return true;
		}
		return false;
	}

	Relation getRelation(string name) {
		return relations[name];
	}

private:
	map<string, Relation> relations;
};

#endif