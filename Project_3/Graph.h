#ifndef GRAPH_H
#define GRAPH_H

#include "Rule.h"
#include <iostream>
#include <algorithm>

class RulesGraph
{
public:
	RulesGraph() {}

	void addRule(Rule newRule) {
		rulesInOrder.push_back(newRule);
	}

	void genAdjacency() {
		adjacencyList = {};

		for (size_t i = 0; i < rulesInOrder.size(); i++) {
			vector<size_t> nbrs = {};
			vector<Predicate> preds = rulesInOrder[i].getPredList();
			for (size_t j = 0; j < preds.size(); j++) {
				string predID = preds[j].id();
				for (size_t k = 0; k < rulesInOrder.size(); k++) {
					if (rulesInOrder[k].head().id() == predID) nbrs.push_back(k);
				}
			}
			adjacencyList.push_back(nbrs);
		}
	}

	void genReverseGraph() {
		reverseDependencies = {};
		for (size_t i = 0; i < adjacencyList.size(); i++) {
			vector<size_t> ancestors = {};
			for (size_t j = 0; j < adjacencyList.size(); j++) {
				for (size_t k = 0; k < adjacencyList[j].size(); k++) {
					if (adjacencyList[j][k] == i) ancestors.push_back(j);
				}
			}
			reverseDependencies.push_back(ancestors);
		}
	}

	vector<size_t> getNbrsForNode(size_t n) {
		return adjacencyList[n];
	}
	vector<size_t> getReverseForNode(size_t n) {
		return reverseDependencies[n];
	}

	vector<vector<size_t>> findSCC() {
		genAdjacency();
		genReverseGraph();
		print();
		vector<size_t> postorder = dfsOfReverseGraph();
		
		vector<vector<size_t>> sccs = {};
		vector<size_t> marked = {};

		for (size_t i = postorder.size(); i > 0; i--) {
			vector<size_t> scc = sccOfNode(adjacencyList, postorder[i-1], marked);
			if (scc.size() > 0) sccs.push_back(scc);
		}

		return sccs;
	}

	vector<size_t> dfsOfReverseGraph() {
		return dfsOfGraph(reverseDependencies);
	}
	vector<size_t> dfsOfOriginalGraph() {
		return dfsOfGraph(adjacencyList);
	}

	void print() {
		for (size_t i = 0; i < adjacencyList.size(); i++) {
			cout << "R" << i << ":";
			for (size_t j = 0; j < adjacencyList[i].size(); j++) {
				cout << " " << adjacencyList[i][j];
			}
			cout << endl;
		}
	}

	void printReverse() {
		for (size_t i = 0; i < reverseDependencies.size(); i++) {
			cout << "R" << i << ":";
			for (size_t j = 0; j < reverseDependencies[i].size(); j++) {
				cout << " " << reverseDependencies[i][j];
			}
			cout << endl;
		}
	}

	string vectorToString(vector<size_t> order) {
		string out = "";
		for (size_t i = 0; i < order.size(); i++) {
			out += " " + to_string(order[i]);
		}
		return out;
	}

	size_t size() {
		return rulesInOrder.size();
	}
private:
	vector<Rule> rulesInOrder;
	vector<vector<size_t>> adjacencyList;
	vector<vector<size_t>> reverseDependencies;



	vector<size_t> dfsOfGraph(vector<vector<size_t>> graph) {
		vector<size_t> preorder = {};
		vector<size_t> postorder = {};

		for (size_t i = 0; i < graph.size(); i++) {
			dfsOfNode(graph, i, preorder, postorder);
		}

		cout << "Preorder:" << vectorToString(preorder) << endl;
		cout << "Postorder:" << vectorToString(postorder) << endl;

		return postorder;
	}

	void dfsOfNode(vector<vector<size_t>>& graph, size_t node_i, vector<size_t>& pre, vector<size_t>& post) {
		vector<size_t>::iterator it = find(pre.begin(), pre.end(), node_i);
		if (it != pre.end()) return;

		pre.push_back(node_i);
		for (size_t j = 0; j < graph[node_i].size(); j++) {
			dfsOfNode(graph, graph[node_i][j], pre, post);
		}
		post.push_back(node_i);
	}

	vector<size_t> sccOfNode(vector<vector<size_t>>& graph, size_t node_i, vector<size_t>& pre) {
		vector<size_t> scc = {};
		
		vector<size_t>::iterator it = find(pre.begin(), pre.end(), node_i);
		if (it != pre.end()) return scc;

		pre.push_back(node_i);
		for (size_t j = 0; j < graph[node_i].size(); j++) {
			vector<size_t> newscc = sccOfNode(graph, graph[node_i][j], pre);
			for (size_t k = 0; k < newscc.size(); k++) scc.push_back(newscc[k]);
		}
		scc.push_back(node_i);
		return scc;
	}


};

#endif