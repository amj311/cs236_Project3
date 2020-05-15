#include "pch.h"
#include <fstream>
#include <string>
using namespace std;

#include "../Project_3/Interpretor.h"

TEST(TEST3, testCompile) {
	EXPECT_TRUE(true);
}


TEST(Tuple, constructFromPred) {
	vector<string> values = { "notString", "'string'", "notString", "'string'" };
	string id = "testPredicate";
	Predicate pred(id, values);

	Tuple tuple(pred.getParamList());
	EXPECT_EQ(tuple[0], "notString");
}

TEST(Tuple, inheritVector) {
	Tuple tuple = Tuple();
	tuple.push_back("1");
	EXPECT_EQ(tuple.front(), "1");

	tuple.push_back("2");
	tuple.push_back("3");
	tuple.push_back("4");
	tuple.pop_back();
	EXPECT_EQ(tuple.back(), "3");
	EXPECT_EQ(tuple.size(), 3);
}

TEST(Relation, constructFromScheme) {
	vector<string> values = { "A", "B", "C" };
	string id = "schemeABC";
	Predicate pred(id, values);

	Relation rel(pred);
	EXPECT_EQ(rel.attrList()[0], "A");
}

TEST(Relation, addTuples) {
	vector<string> values = { "A", "B", "C" };
	string name = "schemeABC";
	Predicate scm(name, values);

	vector<string> params = { "ay", "bee", "sea" };
	string id = "schemeABC";
	Predicate fact(id, params);
	Tuple tuple(fact.getParamList());

	Relation rel(scm);
	rel.insert(tuple);
	EXPECT_EQ(rel.attrList()[0], "A");
	
	EXPECT_EQ(rel.size(), 1);
	rel.insert(tuple);
	EXPECT_EQ(rel.size(), 1);
}


TEST(Database, constructor) {
	Database db = Database();
	EXPECT_FALSE(db.hasRelation("test"));
}

string getFile(string path) {
	ifstream ifs(path);
	return string((istreambuf_iterator<char>(ifs)),
		(istreambuf_iterator<char>()));
}

//#include "../Project_3/Lexer.h"
//
//TEST(parseProgram, testFile) {
//	string file = getFile("../../Project2/testFile.txt");
//
//	Lexer lexer(file);
//	lexer.tokenizeInput();
//
//	Parser parser(lexer.getTokenList());
//	bool res = parser.run();
//	EXPECT_TRUE(res);
//
//	DatalogProg prog = parser.getParsedProgram();
//	EXPECT_EQ(prog.schemeList().size(), 2);
//	EXPECT_EQ(prog.factList().size(), 2);
//	EXPECT_EQ(prog.ruleList().size(), 2);
//	EXPECT_EQ(prog.queryList().size(), 2);
//}
//
//
//
//TEST(parseProgram, successFiles) {
//	vector<int> fileNums = {
//		21, 26, 61, 62
//	};
//
//	for (size_t i = 0; i < fileNums.size(); i++) {
//		cout << endl << "Testing file #" << fileNums[i] << endl << endl;
//
//		string file = getFile("../../Project2/project2-exampleIO/in" + to_string(fileNums[i]) + ".txt");
//		Lexer lexer(file);
//		lexer.tokenizeInput();
//
//		Parser parser(lexer.getTokenList());
//		bool res = parser.run();
//		cout << "File #" << fileNums[i] << ": " << boolalpha << res << endl;
//		cout << "------------------" << endl;
//		EXPECT_TRUE(res);
//	}
//
//}
//
//TEST(parseProgram, failFiles) {
//	vector<int> fileNums = {
//		25, 27, 28, 22, 23, 24
//	};
//
//	for (size_t i = 0; i < fileNums.size(); i++) {
//		cout << endl << "Testing file #" << fileNums[i] << endl << endl;
//
//		string file = getFile("../../Project2/project2-exampleIO/in" + to_string(fileNums[i]) + ".txt");
//		Lexer lexer(file);
//		lexer.tokenizeInput();
//
//		Parser parser(lexer.getTokenList());
//		bool res = parser.run();
//		cout << "File #" << fileNums[i] << ": " << boolalpha << res << endl;
//		cout << "------------------" << endl;
//		EXPECT_FALSE(res);
//	}
//
//}

