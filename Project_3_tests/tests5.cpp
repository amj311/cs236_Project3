#include "pch.h"
#include <fstream>
#include <string>
using namespace std;

#include "../Project_3/Interpretor.h"

vector<string> values = { "X", "Y" };

string nameA = "A";
Predicate predA(nameA, values);

string nameB = "B";
Predicate predB(nameB, values);

string nameC = "C";
Predicate predC(nameC, values);

string nameD = "D";
Predicate predD(nameD, values);

string nameE = "E";
Predicate predE(nameE, values);

TEST(TEST5, testCompile) {
	EXPECT_TRUE(true);
}


TEST(RulesGraph, addRules) {
	vector<Predicate> preds_BC = { predB, predC };
	Rule ruleA_BC(predA, preds_BC);

	vector<Predicate> preds_AD = { predA, predD };
	Rule ruleB_AD(predB, preds_AD);

	RulesGraph graph = RulesGraph();
	graph.addRule(ruleA_BC);
	graph.addRule(ruleB_AD);

	EXPECT_EQ(graph.size(), 2);
}

TEST(RulesGraph, graph_1x1) {
	vector<Predicate> preds_B = { predB };
	Rule ruleA_B(predA, preds_B);

	vector<Predicate> preds_A = { predA };
	Rule ruleB_A(predB, preds_A);

	RulesGraph graph = RulesGraph();
	graph.addRule(ruleA_B);
	graph.addRule(ruleB_A);
	graph.genAdjacency();
	graph.print();

	EXPECT_EQ(graph.size(), 2);
	EXPECT_EQ(graph.getNbrsForNode(0)[0], 1);
	EXPECT_EQ(graph.getNbrsForNode(1)[0], 0);
}

TEST(RulesGraph, graph_roundRobin){
	vector<Predicate> preds_B = { predB };
	Rule ruleA_B(predA, preds_B);

	vector<Predicate> preds_C = { predC };
	Rule ruleB_C(predB, preds_C);

	vector<Predicate> preds_D = { predD };
	Rule ruleC_D(predC, preds_D);

	vector<Predicate> preds_A = { predA };
	Rule ruleD_A(predD, preds_A);

	RulesGraph graph = RulesGraph();
	graph.addRule(ruleA_B);
	graph.addRule(ruleB_C);
	graph.addRule(ruleC_D);
	graph.addRule(ruleD_A);
	graph.genAdjacency();
	graph.print();

	EXPECT_EQ(graph.size(), 4);
	EXPECT_EQ(graph.getNbrsForNode(0)[0], 1);
	EXPECT_EQ(graph.getNbrsForNode(1)[0], 2);
	EXPECT_EQ(graph.getNbrsForNode(2)[0], 3);
	EXPECT_EQ(graph.getNbrsForNode(3)[0], 0);
}

TEST(RulesGraph, graph_doubleHead) {
	vector<Predicate> preds_B = { predB };
	Rule ruleA_B(predA, preds_B);

	vector<Predicate> preds_C = { predC };
	Rule ruleA_C(predA, preds_C);

	vector<Predicate> preds_A = { predA };
	Rule ruleB_A(predB, preds_A);

	Rule ruleC_A(predC, preds_A);

	RulesGraph graph = RulesGraph();
	graph.addRule(ruleA_B);
	graph.addRule(ruleA_C);
	graph.addRule(ruleB_A);
	graph.addRule(ruleC_A);
	graph.genAdjacency();
	graph.print();

	EXPECT_EQ(graph.size(), 4);
	EXPECT_EQ(graph.getNbrsForNode(0)[0], 2);
	EXPECT_EQ(graph.getNbrsForNode(1)[0], 3);
	EXPECT_EQ(graph.getNbrsForNode(2)[0], 0);
	EXPECT_EQ(graph.getNbrsForNode(2)[1], 1);
	EXPECT_EQ(graph.getNbrsForNode(3)[0], 0);
	EXPECT_EQ(graph.getNbrsForNode(3)[1], 1);
}

TEST(RulesGraph, graph_doubleHead_reverse) {
	vector<Predicate> preds_B = { predB };
	Rule ruleA_B(predA, preds_B);

	vector<Predicate> preds_C = { predC };
	Rule ruleA_C(predA, preds_C);

	vector<Predicate> preds_A = { predA };
	Rule ruleB_A(predB, preds_A);

	Rule ruleC_A(predC, preds_A);

	RulesGraph graph = RulesGraph();
	graph.addRule(ruleA_B);
	graph.addRule(ruleA_C);
	graph.addRule(ruleB_A);
	graph.addRule(ruleC_A);
	graph.genAdjacency();
	graph.genReverseGraph();
	graph.printReverse();

}



TEST(RulesGraph, graph_ls_example) {
	vector<Predicate> preds_B = { predB };
	Rule ruleA_B(predA, preds_B);

	vector<Predicate> preds_A = { predA };
	Rule ruleB_A(predB, preds_A);

	Rule ruleB_B(predB, preds_B);

	vector<Predicate> preds_D = { predD };
	Rule ruleE_D(predE, preds_D);

	vector<Predicate> preds_E = { predE };
	Rule ruleE_E(predE, preds_E);

	RulesGraph graph = RulesGraph();
	graph.addRule(ruleA_B);
	graph.addRule(ruleB_A);
	graph.addRule(ruleB_B);
	graph.addRule(ruleE_D);
	graph.addRule(ruleE_E);
	graph.genAdjacency();
	graph.genReverseGraph();
	graph.printReverse();
	vector<size_t> postorder = graph.dfsOfReverseGraph();
	vector<size_t> correctOrder = { 2, 1, 0, 4, 3 };
	EXPECT_EQ(postorder, correctOrder);
}

TEST(RulesGraph, bob_scc) {
	vector<Predicate> preds_B = { predB };
	Rule ruleB_B(predB, preds_B);

	vector<Predicate> preds_D = { predD };
	Rule ruleB_D(predB, preds_D);

	Rule ruleD_B(predD, preds_B);

	RulesGraph graph = RulesGraph();
	graph.addRule(ruleB_B);
	graph.addRule(ruleB_D);
	graph.addRule(ruleD_B);
	vector<vector<size_t>> sccs = graph.findSCC();

	vector<vector<size_t>> correctSccs = { {0, 1, 2} };
	EXPECT_EQ(sccs, correctSccs);
}




string getFile(string path) {
	ifstream ifs(path);
	return string((istreambuf_iterator<char>(ifs)),
		(istreambuf_iterator<char>()));
}

#include "../Project_3/Lexer.h"
#include "../Project_3/Parser.h"

TEST(Interpretor, testFile) {
	string file = getFile("../Project_3/testFile.txt");
	//cout << file << endl;

	Lexer lexer(file);
	lexer.tokenizeInput();

	Parser parser(lexer.getTokenList());

	parser.parseProgram();
	DatalogProg prog = parser.getParsedProgram();

	Interpretor interp = Interpretor();
	interp.interpretProgram(prog);
}