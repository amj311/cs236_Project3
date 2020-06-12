#ifndef INTERP_H
#define INTERP_H

#include "DatalogProg.h"
#include "Database.h"
#include "Graph.h"
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

		// create tuples for schemes, add to relation in database
		vector<Predicate> facts = prog.factList();
		for (size_t i = 0; i < facts.size(); i++) {
			db.saveFact(facts[i]);
		}

		rules = prog.ruleList();
		queries = prog.queryList();
	}

	void interpretLoaded() {
		RulesGraph graph = RulesGraph();
		for (size_t i = 0; i < rules.size(); i++) {
			graph.addRule(rules[i]);
		}
		vector<vector<size_t>> sccs = graph.findSCC();
		for (size_t i = 0; i < sccs.size(); i++) {
			vector<Rule> sccRules = {};
			for (size_t j = 0; j < sccs[i].size(); j++) {
				sccRules.push_back(rules[sccs[i][j]]);
			}

			interpretRules(sccRules);
		}

		interpretRules(rules);

		cout << "Query Evaluation" << endl;
		for (size_t i = 0; i < queries.size(); i++) {
			evalPred(queries[i], true);
		}
	}

	void interpretRules(vector<Rule> ruleList) {
		cout << "Rule Evaluation" << endl;
		int rulePasses = 0;
		bool addedTuples;
		string out;
		do {
			addedTuples = false;
			out = "";

			for (size_t i = 0; i < ruleList.size(); i++) {
				cout << ruleList[i].toString() << "\n";
				Relation* dbRel = db.getRelationPtr(ruleList[i].head().id());
				vector<Predicate> preds = ruleList[i].getPredList();

				Relation ruleRelation = evalPred(preds[0]);
				
				for (size_t j = 0; j < preds.size(); j++) {
					ruleRelation = ruleRelation.join(evalPred(preds[j]));
				}

				vector<string> headScheme = ruleList[i].head().getParamValues();
				vector<size_t> projCols;
				for (size_t i = 0; i < headScheme.size(); i++) {
					for (size_t j = 0; j < ruleRelation.getHeader().scheme.size(); j++) {
						if (ruleRelation.getHeader().scheme[j] == headScheme[i]) {
							projCols.push_back(j);
							ruleRelation = ruleRelation.rename(j, (*dbRel).getHeader().scheme[i]);
						}
					}
				}
				ruleRelation = ruleRelation.project(projCols);
				if ((*dbRel).addFrom(ruleRelation)) addedTuples = true;
			}

			rulePasses++;
		} while (addedTuples);

		cout << endl << "Schemes populated after " << rulePasses << " passes through the Rules." << endl << endl;
	}

	Relation getRelation(string name) {
		if (db.hasRelation(name)) {
			return db.getRelation(name);
		}
		else return Relation();
	}

	Relation relFromQuery(size_t pos) {
		return evalPred(queries[pos]);
	}

private:
	Database db = Database();
	vector<Rule> rules;
	vector<Predicate> queries;

	Relation evalPred(Predicate query, bool print = false) {
		if (print) cout << query.toString() << "? ";

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



		if (print) {
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
		}

		return finalRel;
	}


};

#endif