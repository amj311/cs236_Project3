#ifndef INTERP_H
#define INTERP_H

#include "DatalogProg.h"
#include "Database.h"
#include <algorithm>

class Interpretor
{
public:
	Interpretor() {};

	void interpretProgram(DatalogProg prog) {
		loadProgram(prog);
		interpretLoaded();
	}


	void loadProgram(DatalogProg prog) {
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
		queries = prog.queryList();
	}

	void interpretLoaded() {
		for (size_t i = 0; i < queries.size(); i++) {
			evalQuery(queries[i]);
		}
	}


	Relation getRelation(string name) {
		if (db.hasRelation(name)) {
			return db.getRelation(name);
		}
		else return Relation();
	}

	Relation relFromQuery(size_t pos) {
		return evalQuery(queries[pos]);
	}

private:
	Database db = Database();
	vector<Predicate> queries;

	Relation evalQuery(Predicate query) {
		cout << query.toString() << "? ";

		vector<Parameter> params = query.getParamList();
		Relation initRel = getRelation(query.id());
		Relation finalRel = initRel;

		for (size_t i = 0; i < params.size(); i++) {
			if (params[i].isSTRING()) {
				//if (finalRel.size() == 0) finalRel = initRel.select(i,params[i].value());
				finalRel = finalRel.select(i, params[i].value());
			}
		}

		vector<size_t> varCols;
		set<string> vars;
		for (size_t i = 0; i < params.size(); i++) {
			if (params[i].isID()) {
				varCols.push_back(i);
				if (vars.find(params[i]) == vars.end()) vars.insert(params[i].value());
			}
		}
		if (varCols.size() > 0) {

			//rename
			for (set<string>::iterator i = vars.begin(); i != vars.end(); i++) {
				vector<size_t> cols;
				for (size_t p = 0; p < params.size(); p++) {
					if (params[p].value() == *i) finalRel = finalRel.rename(p, *i);
				}
			}


			// Project all vars
			//if (finalRel.size() == 0) finalRel = initRel.project(varCols);
			finalRel = finalRel.project(varCols);


			// Select Matching Variables
			for (set<string>::iterator i = vars.begin(); i != vars.end(); i++) {
				vector<size_t> cols;
				for (size_t p = 0; p < finalRel.getHeader().scheme.size(); p++) {
					if (finalRel.getHeader().scheme[p] == *i) cols.push_back(p);
				}
				if (cols.size() > 0) finalRel = finalRel.selectIfEqual(cols);
			}

			// Remove duplicate vars
			vector<size_t> uniqueCols;
			set<string> checkedVars;
			vector<string> scheme = finalRel.getHeader().scheme;
			for (size_t p = 0; p < scheme.size(); p++) {
				if (checkedVars.find(scheme[p]) == checkedVars.end()) {
					uniqueCols.push_back(p);
					checkedVars.insert(scheme[p]);
				}
			}
			if (uniqueCols.size() > 0) finalRel = finalRel.project(uniqueCols);
		}




		// PRINT RESULTS
		if (finalRel.size() > 0) {
			cout << "Yes(" << finalRel.size() << ")" << endl;
			if (varCols.size() > 0) {
				for (Relation::iterator it = finalRel.begin(); it != finalRel.end(); it++) {
					cout << "  ";
					for (size_t h = 0; h < finalRel.getHeader().scheme.size(); h++) {
						cout << finalRel.getHeader().scheme[h] << "=" << (*it)[h];
						if (h < finalRel.getHeader().scheme.size() - 1) cout << ", ";
					}
					cout << endl;
				}
			}
		}
		else cout << "No" << endl;

		return finalRel;
	}


};

#endif