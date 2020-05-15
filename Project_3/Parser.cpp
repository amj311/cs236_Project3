#include "Parser.h"


void Parser::printFail(Token& badToken) {
	cout << "Failure!" << endl << "  " << badToken.toString() << endl;
}

void Parser::printSuccess() {
	cout << "Success!" << endl;
}

bool Parser::tryAdvanceIfType(TOKEN_TYPE typeAssert, bool expectEOF) {
	if (curTokenIs(typeAssert, expectEOF)) {
		// Do parsing stuff.
		if (typeAssert != EOF_TYPE) advanceToken(expectEOF);
		return true;
	}
	else return throwBadToken(curToken());
}

bool Parser::run() {
	try
	{
		parseProgram();
	}
	catch (Token badToken) {
		printFail(badToken);
		return false;
	}
	return true;
}

bool Parser::parseProgram() {
	prog = DatalogProg();
	tokenIdx = 0;
	while (curTokenIs(COMMENT)) tokenIdx++;

	tryAdvanceIfType(SCHEMES);
	tryAdvanceIfType(COLON);
	parse_scheme();
	parse_schemeList();
	
	tryAdvanceIfType(FACTS);
	tryAdvanceIfType(COLON);
	parse_factList();

	tryAdvanceIfType(RULES);
	tryAdvanceIfType(COLON);
	parse_ruleList();

	tryAdvanceIfType(QUERIES);
	tryAdvanceIfType(COLON);

	parse_query();
	parse_queryList();

	tryAdvanceIfType(EOF_TYPE, true);

	prog.setReady();

	printSuccess();
	cout << prog.toString();
	return true;
}


vector<Token> Parser::repeatListOfType(TOKEN_TYPE listType) {
	vector<Token> list;

	while (curTokenIs(COMMA)) {
		advanceToken();
		if (curTokenIs(listType)) {
			list.push_back(curToken());
			advanceToken();
		}
		else {
			throwBadToken(curToken());
			break;
		}
	}
	return list;
}

bool Parser::parse_schemeList() {
	while (!curTokenIs(FACTS)) parse_scheme();
	return true;
}
bool Parser::parse_scheme() {
	string id = curToken().getValue();
	tryAdvanceIfType(ID);
	tryAdvanceIfType(LEFT_PAREN);
	vector<string> params = { curToken().getValue() };
	tryAdvanceIfType(ID);
	vector<Token> paramTokens = repeatListOfType(ID);
	tryAdvanceIfType(RIGHT_PAREN);

	for (size_t i = 0; i < paramTokens.size(); i++) {
		params.push_back(paramTokens[i].getValue());
	}

	prog.addScheme(id, params);

	return true;
}

bool Parser::parse_factList() {
	while (!curTokenIs(RULES)) parse_fact();
	return true;
}
bool Parser::parse_fact() {
	string id = curToken().getValue();
	tryAdvanceIfType(ID);
	tryAdvanceIfType(LEFT_PAREN);
	vector<string> params = { curToken().getValue() };
	tryAdvanceIfType(STRING);
	vector<Token> paramTokens = repeatListOfType(STRING);
	tryAdvanceIfType(RIGHT_PAREN);
	tryAdvanceIfType(PERIOD);

	for (size_t i = 0; i < paramTokens.size(); i++) {
		params.push_back(paramTokens[i].getValue());
	}
	
	prog.addFact(id, params);

	return true;
}

bool Parser::parse_ruleList() {
	while (!curTokenIs(QUERIES)) parse_rule();
	return true;
}
bool Parser::parse_rule() {
	predPair headPair = parse_headPredicate();
	tryAdvanceIfType(COLON_DASH);
	
	vector<predPair> preds = { parse_predicate() };
	vector<predPair> predList = parse_predicateList();
	tryAdvanceIfType(PERIOD);

	for (size_t i = 0; i < predList.size(); i++) {
		preds.push_back(predList[i]);
	}

	prog.addRule(headPair.first, headPair.second, preds);

	return true;
}

predPair Parser::parse_headPredicate() {
	predPair pair;
	pair.first = curToken().getValue();
	tryAdvanceIfType(ID);
	tryAdvanceIfType(LEFT_PAREN);
	
	vector<string> params = { curToken().getValue() };
	tryAdvanceIfType(ID);
	vector<Token> paramTokens = repeatListOfType(ID);
	tryAdvanceIfType(RIGHT_PAREN);

	for (size_t i = 0; i < paramTokens.size(); i++) {
		params.push_back(paramTokens[i].getValue());
	}

	pair.second = params;

	return pair;
}
vector<predPair> Parser::parse_predicateList() {
	vector<predPair> pairs;
	
	while (curTokenIs(COMMA)) {
		advanceToken();
		pairs.push_back(parse_predicate());
	}
	return pairs;
}
predPair Parser::parse_predicate() {
	predPair pair;
	pair.first = curToken().getValue();
	
	tryAdvanceIfType(ID);
	tryAdvanceIfType(LEFT_PAREN);
	expectParameter();
	vector<string> params = parse_parameterList();
	tryAdvanceIfType(RIGHT_PAREN);

	pair.second = params;
	
	return pair;
}

vector<string> Parser::parse_parameterList() {
	vector<string> list;

	if (!expectParameter()) throwBadToken(curToken());

	list.push_back(curToken().getValue());
	advanceToken();

	while (curTokenIs(COMMA)) {
		advanceToken();

		if (expectParameter()) {
			list.push_back(curToken().getValue());
			advanceToken();
		}
		else {
			throwBadToken(curToken());
			break;
		}
	}

	return list;
}
bool Parser::expectParameter() {
	if (curTokenIs(STRING) || curTokenIs(ID)) {
		return true;
	}
	throw curToken();
	return false;
}

bool Parser::parse_queryList() {
	while (!curTokenIs(EOF_TYPE, true)) parse_query();
	return true;
}
bool Parser::parse_query() {
	predPair pair = parse_predicate();
	tryAdvanceIfType(Q_MARK, true);

	prog.addQuery(pair.first, pair.second);
	
	return true;
}
