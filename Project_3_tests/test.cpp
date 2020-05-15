#include "pch.h"

#include "../Project_3/Parser.h"

TEST(matchType, SCHEMES_bad) {
	vector<Token*> tokens = {
		new Token()
	};
	Parser parser(tokens);
	EXPECT_THROW(parser.tryAdvanceIfType(SCHEMES), Token);
}
TEST(matchType, SCHEMES_good) {
	vector<Token*> tokens = {
		new Token(SCHEMES,"SCHEMES",1)
	};
	Parser parser(tokens);
	EXPECT_TRUE(parser.tryAdvanceIfType(SCHEMES));
}

TEST(SkipComments, repeatList) {
	vector<Token*> tokens = {
		new Token(COMMA,",",1),
		new Token(ID,"S",1),
		new Token(COMMENT,"#,",1),
		new Token(COMMENT,"#,",1),
		new Token(COMMENT,"#,",1),
		new Token(COMMA,",",1),
		new Token(ID,"N",1),
		new Token(RIGHT_PAREN,")",1),
	};
	Parser parser(tokens);
	vector<Token> list = parser.repeatListOfType(ID);
	EXPECT_EQ(list.size(), 2);
}

TEST(repeatList, ID_good) {
	vector<Token*> tokens = {
		new Token(COMMA,",",1),
		new Token(ID,"S",1),
		new Token(COMMA,",",1),
		new Token(ID,"N",1),
		new Token(RIGHT_PAREN,")",1),
	};
	Parser parser(tokens);
	vector<Token> list = parser.repeatListOfType(ID);
	EXPECT_EQ(list[0].toString(), "(ID,\"S\",1)");
}
TEST(repeatList, ID_bad) {
	vector<Token*> tokens = {
		new Token(COMMA,",",1),
		new Token(ID,"S",1),
		new Token(ID,"N",1),
		new Token(RIGHT_PAREN,")",1),
	};
	Parser parser(tokens);
	vector<Token> list = parser.repeatListOfType(ID);
	EXPECT_EQ(list.size(), 1);
}
TEST(repeatList, ID_bad_wrongType) {
	vector<Token*> tokens = {
		new Token(COMMA,",",1),
		new Token(ID,"S",1),
		new Token(COMMA,",",1),
		new Token(STRING,"'N'",1),
		new Token(RIGHT_PAREN,")",1),
	};
	Parser parser(tokens);
	EXPECT_THROW(parser.repeatListOfType(ID), Token);
}

TEST(expectParameter, ID) {
	vector<Token*> tokens = {
		new Token(ID,"A",1)
	};
	Parser parser(tokens);
	EXPECT_TRUE(parser.expectParameter());
}

TEST(parsePredicate, IDIDID_NoComma) {
	vector<Token*> tokens = {
		new Token(ID,"pred",1),
		new Token(LEFT_PAREN,"(",1),
		new Token(ID,"A",1),
		new Token(ID,"X",1),
		new Token(ID,"B",1),
		new Token(RIGHT_PAREN,")",1),
	};
	Parser parser(tokens);
	EXPECT_EQ(parser.repeatListOfType(ID).size(), 0);
}
TEST(parsePredicate, IDIDID_Good) {
	vector<Token*> tokens = {
		new Token(COMMA,",",1),
		new Token(ID,"A",1),
		new Token(COMMA,",",1),
		new Token(ID,"X",1),
		new Token(COMMA,",",1),
		new Token(ID,"B",1),
		new Token(RIGHT_PAREN,")",1),
	};
	Parser parser(tokens);
	EXPECT_EQ(parser.repeatListOfType(ID).size(), 3);
}



TEST(Parameter, STRING) {
	string value = "'string'";
	Parameter param(value);
	EXPECT_FALSE(param.isID());
}
TEST(Parameter, ID) {
	string value = "notString";
	Parameter param(value);
	EXPECT_TRUE(param.isID());
}


TEST(Predicate, constructor) {
	vector<string> values = { "notString", "'string'", "notString", "'string'" };
	string id = "testPredicate";
	Predicate pred(id, values);
	EXPECT_EQ(pred.id(), "testPredicate");
	EXPECT_EQ(pred.toString(), "testPredicate(notString,'string',notString,'string')");

	vector<Parameter> params = pred.getParamList();
	EXPECT_EQ(params[0].value(), "notString");
	EXPECT_TRUE(params[0].isID());
	EXPECT_EQ(params[3].value(), "'string'");
	EXPECT_TRUE(params[3].isSTRING());
}




TEST(Rule, constructor) {
	vector<string> values0 = { "X", "Y" };
	string id0 = "HasSameAddress";
	Predicate headPred(id0, values0);

	vector<string> values1 = { "a", "b" };
	string id1 = "snap";

	vector<string> values2 = { "c", "d" };
	string id2 = "snap";

	vector<Predicate> preds = {
		Predicate(id1, values1),
		Predicate(id2, values2),
	};

	Rule rule(headPred, preds);

	EXPECT_EQ(rule.toString(), "HasSameAddress(X,Y) :- snap(a,b),snap(c,d).");
}






TEST(Program, addScheme) {
	vector<string> values0 = { "s", "c", "m" };
	string id0 = "scm";
	DatalogProg prog = DatalogProg();
	prog.addScheme(id0, values0);
	EXPECT_EQ(prog.schemeList()[0].toString(), "scm(s,c,m)");
}

TEST(Program, addFact) {
	vector<string> values0 = { "f", "c", "t" };
	string id0 = "fct";
	DatalogProg prog = DatalogProg();
	prog.addFact(id0, values0);
	EXPECT_EQ(prog.factList()[0].toString(), "fct(f,c,t)");
}

TEST(Program, sortedDomain) {
	vector<string> values0 = { "'z'", "'1'", "'a'" };
	string id0 = "fct";

	DatalogProg prog = DatalogProg();
	prog.addFact(id0, values0);

	EXPECT_EQ(prog.toString(), "Schemes(0):\nFacts(1):\n  fct('z','1','a').\nRules(0):\nQueries(0):\nDomain(3):\n  '1'\n  'a'\n  'z'\n");
}

TEST(Program, addRule) {
	vector<string> values0 = { "h", "'d'", "p" };
	string id0 = "hdp";

	pair<string, vector<string>> pd1;
	pd1.first = "pd1";
	vector<string> params1 = { "p","d","1" };
	pd1.second = params1;

	vector<pair<string, vector<string>>> predPairs = { pd1 };

	DatalogProg prog = DatalogProg();
	prog.addRule(id0, values0, predPairs);
	EXPECT_EQ(prog.ruleList()[0].toString(), "hdp(h,'d',p) :- pd1(p,d,1).");
}

TEST(Program, addQuery) {
	vector<string> values0 = { "q", "r", "y" };
	string id0 = "qry";
	DatalogProg prog = DatalogProg();
	prog.addQuery(id0, values0);
	EXPECT_EQ(prog.queryList()[0].toString(), "qry(q,r,y)");
}



TEST(parseProgram, throwErrorBeforeEnd) {
	vector<Token*> tokens = {
		new Token(SCHEMES,"Schemes",1),
		new Token(COLON,":",1),
		new Token(ID,"S",2),
		new Token(LEFT_PAREN,"(",2),
		new Token(EOF_TYPE,"",3),
	};
	Parser parser(tokens);
	EXPECT_THROW(parser.parseProgram(), Token);
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


