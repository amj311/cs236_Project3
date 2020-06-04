#include "pch.h"
#include <fstream>
#include <string>
using namespace std;

#include "../Project_3/Interpretor.h"

TEST(TEST4, testCompile) {
	EXPECT_TRUE(true);
}


TEST(Header, combine) {
	vector<string> firstS = { "one", "two", "three" };
	Header firstH = Header(firstS);
	vector<string> secondS = { "two", "four" };
	Header secondH = Header(secondS);

	Header newH = firstH.combineWithHeader(secondH);
	EXPECT_EQ(newH.scheme.size(), 4);
	EXPECT_EQ(newH.scheme[3], "four");
}

TEST(Header, duplicates) {
	vector<string> firstS = { "one", "two", "three", "one" };
	Header firstH = Header(firstS);
	vector<string> secondS = { "two", "four", "three" };
	Header secondH = Header(secondS);

	Header newH = firstH.combineWithHeader(secondH);
	EXPECT_EQ(newH.scheme.size(), 5);
	EXPECT_EQ(newH.scheme[4], "four");
}

TEST(Header, sameHeader) {
	vector<string> firstS = { "one", "two", "three" };
	Header firstH = Header(firstS);
	vector<string> secondS = { "two", "two", "three" };
	Header secondH = Header(secondS);

	Header newH = firstH.combineWithHeader(secondH);
	EXPECT_EQ(newH.scheme.size(), 3);
}

TEST(Header, emptyHeader) {
	vector<string> firstS = { "one", "two", "three" };
	Header firstH = Header(firstS);
	vector<string> secondS = {  };
	Header secondH = Header(secondS);

	Header newH = firstH.combineWithHeader(secondH);
	EXPECT_EQ(newH.scheme.size(), 3);
}


TEST(Relation, joinAC) {
	vector<string> scm1cols = { "A", "B", "C" };
	string scm1name = "schemeABC";

	vector<string> params = { "ay", "ay", "ay" };
	Predicate fact(scm1name, params);
	Tuple tuple(fact.getParamList());

	vector<string> scm2cols = { "C", "A", "D" };
	string scm2name = "schemeCAD";

	vector<string> params2 = { "ay", "ay", "bee" };
	Predicate fact2(scm2name, params2);
	Tuple tuple2(fact2.getParamList());

	Relation rel1(scm1name, scm1cols);
	rel1.insert(tuple);
	Relation rel2(scm2name, scm2cols);
	rel2.insert(tuple2);

	Relation res = rel1.join(rel2);
	EXPECT_EQ(res.size(), 1);
}



TEST(Relation, joinAC_shrink) {
	vector<string> scm1cols = { "A", "B", "C" };
	string scm1name = "schemeABC";

	vector<string> params = { "ay", "ay", "ay" };
	Predicate fact(scm1name, params);
	Tuple tuple(fact.getParamList());

	vector<string> scm2cols = { "C", "A", "D" };
	string scm2name = "schemeCAD";

	vector<string> params2 = { "ay", "ay", "bee" };
	Predicate fact2(scm2name, params2);
	Tuple tuple2(fact2.getParamList());

	vector<string> params3 = { "sea", "ay", "bee" };
	Predicate fact3(scm2name, params3);
	Tuple tuple3(fact3.getParamList());

	Relation rel1(scm1name, scm1cols);
	rel1.insert(tuple);
	Relation rel2(scm2name, scm2cols);
	rel2.insert(tuple2);
	rel2.insert(tuple3);

	Relation res = rel1.join(rel2);
	EXPECT_EQ(res.size(), 1);
}


TEST(Relation, join_uniques) {
	vector<string> scm1cols = { "A", "B", "C" };
	string scm1name = "schemeABC";

	vector<string> params = { "ay", "ay", "ay" };
	Predicate fact(scm1name, params);
	Tuple tuple(fact.getParamList());

	vector<string> scm2cols = { "D", "E" };
	string scm2name = "schemeDE";

	vector<string> params2 = { "ay", "bee" };
	Predicate fact2(scm2name, params2);
	Tuple tuple2(fact2.getParamList());

	Relation rel1(scm1name, scm1cols);
	rel1.insert(tuple);
	Relation rel2(scm2name, scm2cols);
	rel2.insert(tuple2);

	Relation res = rel1.join(rel2);
	EXPECT_EQ(res.size(), 1);
	EXPECT_EQ(res.getHeader().scheme.size(), 5);
}



TEST(Relation, union_mismatch) {
	vector<string> scm1cols = { "A", "B", "C" };
	string scm1name = "schemeABC";

	vector<string> params = { "ay", "ay", "ay" };
	Predicate fact(scm1name, params);
	Tuple tuple(fact.getParamList());

	vector<string> scm2cols = { "D", "E" };
	string scm2name = "schemeDE";

	vector<string> params2 = { "ay", "bee" };
	Predicate fact2(scm2name, params2);
	Tuple tuple2(fact2.getParamList());

	Relation rel1(scm1name, scm1cols);
	rel1.insert(tuple);
	Relation rel2(scm2name, scm2cols);
	rel2.insert(tuple2);

	bool res = rel1.addFrom(rel2);
	EXPECT_EQ(res, false);
	EXPECT_EQ(rel1.size(), 1);
}



TEST(Relation, union_once) {
	vector<string> scm1cols = { "A", "B", "C" };
	string scm1name = "schemeABC";

	vector<string> params = { "ay", "ay", "ay" };
	Predicate fact(scm1name, params);
	Tuple tuple(fact.getParamList());

	vector<string> scm2cols = { "A", "B", "C" };
	string scm2name = "schemeABC2";

	vector<string> params2 = { "ay", "bee", "sea" };
	Predicate fact2(scm2name, params2);
	Tuple tuple2(fact2.getParamList());

	Relation rel1(scm1name, scm1cols);
	rel1.insert(tuple);
	Relation rel2(scm2name, scm2cols);
	rel2.insert(tuple2);

	bool res = rel1.addFrom(rel2);
	EXPECT_EQ(res, true);
	EXPECT_EQ(rel1.size(), 2);
}



TEST(Relation, union_twice) {
	vector<string> scm1cols = { "A", "B", "C" };
	string scm1name = "schemeABC";

	vector<string> params = { "ay", "ay", "ay" };
	Predicate fact(scm1name, params);
	Tuple tuple(fact.getParamList());

	vector<string> scm2cols = { "A", "B", "C" };
	string scm2name = "schemeABC2";

	vector<string> params2 = { "ay", "bee", "sea" };
	Predicate fact2(scm2name, params2);
	Tuple tuple2(fact2.getParamList());

	Relation rel1(scm1name, scm1cols);
	rel1.insert(tuple);
	Relation rel2(scm2name, scm2cols);
	rel2.insert(tuple2);

	rel1.addFrom(rel2);
	bool res = rel1.addFrom(rel2);
	EXPECT_EQ(res, false);
	EXPECT_EQ(rel1.size(), 2);
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

	//Relation res = interp.relFromQuery(0);
	//EXPECT_EQ(res.size(), 1);
	//
	//res = interp.relFromQuery(1);
	//EXPECT_EQ(res.size(), 2);
}
