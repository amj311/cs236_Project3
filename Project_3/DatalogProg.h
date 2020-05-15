#ifndef DLP_H
#define DLP_H

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"
using namespace std;

class DatalogProg
{
public:
    DatalogProg() {};
    bool isReady = false;

    void addScheme(string id, vector<string> params) {
        schemes.push_back(Predicate(id, params));
    }
    void addFact(string id, vector<string> params) {
        facts.push_back(Predicate(id, params));
        for (size_t i = 0; i < params.size(); i++) {
            domain.insert(params[i]);
        }
    }
    void addRule(string headId, vector<string> headParams, vector<pair<string, vector<string>>> predPairs) {
        Predicate headPred(headId, headParams);
        vector<Predicate> preds;
        for (size_t i = 0; i < predPairs.size(); i++) {
            preds.push_back(Predicate(predPairs[i].first, predPairs[i].second));
        }
        rules.push_back(Rule(headPred, preds));
    }
    void addQuery(string id, vector<string> params) {
        queries.push_back(Predicate(id, params));
    }


    vector<Predicate> schemeList() { return schemes; }
    vector<Predicate> factList() { return facts; }
    vector<Rule> ruleList() { return rules; }
    vector<Predicate> queryList() { return queries; }

    string toString() {
        string out = "";
        out += "Schemes(" + to_string(schemes.size()) + "):\n";
        for (size_t i = 0; i < schemes.size(); i++) {
            out += "  " + schemes[i].toString() + "\n";
        }

        out += "Facts(" + to_string(facts.size()) + "):\n";
        for (size_t i = 0; i < facts.size(); i++) {
            out += "  " + facts[i].toString() + ".\n";
        }

        out += "Rules(" + to_string(rules.size()) + "):\n";
        for (size_t i = 0; i < rules.size(); i++) {
            out += "  " + rules[i].toString() + "\n";
        }

        out += "Queries(" + to_string(queries.size()) + "):\n";
        for (size_t i = 0; i < queries.size(); i++) {
            out += "  " + queries[i].toString() + "?\n";
        }

        out += "Domain(" + to_string(domain.size()) + "):\n";
        for (set<string>::iterator i = domain.begin(); i != domain.end(); i++) {
            out += "  " + *i + "\n";
        }
        return out;
    }

    void setReady() { isReady = true; }

private:
    vector<Predicate> schemes;
    vector<Predicate> facts;
    vector<Rule> rules;
    vector<Predicate> queries;
    set<string> domain;
};





#endif