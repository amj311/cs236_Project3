#ifndef INTERP_H
#define INTERP_H

#include "DatalogProg.h"
#include "Database.h"

class Interpretor
{
public:
	Interpretor(DatalogProg prog) {
		// create Relations for schemes, add to database
		vector<Predicate> schemes = prog.schemeList();
		for (size_t i = 0; i < schemes.size(); i++) {
			db.addRelationFromScheme(schemes[i]);
		}
		// creat tuples for schemes, add to relation in database
		vector<Predicate> facts = prog.factList();
		for (size_t i = 0; i < facts.size(); i++) {
			db.saveFact(facts[i]);
		}
	};

	Relation getRelation(string name) {
		if (db.hasRelation(name)) {
			return db.getRelation(name);
		}
		else return Relation();
	}

private:
	Database db = Database();
};

#endif