#ifndef PARSER_H
#define PAERSER_H

#include "DatalogProg.h"
#include "Token.h"

using predPair = pair<string, vector<string>>;

class Parser
{
public:
	Parser(vector<Token*> tokenInput) : tokens(tokenInput) {};
	bool run();
	bool parseProgram();

	DatalogProg getParsedProgram() { return prog; }

	vector<Token*> tokens;

	bool tryAdvanceIfType(TOKEN_TYPE typeAssert, bool expectEOF = false);
	vector<Token> repeatListOfType(TOKEN_TYPE);

	bool parse_schemeList();
	bool parse_scheme();

	bool parse_factList();
	bool parse_fact();

	bool parse_ruleList();
	bool parse_rule();

	bool parse_queryList();
	bool parse_query();

	predPair parse_headPredicate();
	vector<predPair> parse_predicateList();
	predPair parse_predicate();
	
	bool expectParameter();
	vector<string> parse_parameterList();

private:
	size_t tokenIdx = 0;

	Token curToken(bool expectEOF = false) {
		Token curToken = *tokens[tokenIdx];
		if (!expectEOF && curToken.getType() == EOF_TYPE) throwBadToken(curToken);
		return curToken;
	}

	TOKEN_TYPE curTokenType() {
		return curToken().getType();
	}
	bool curTokenIs(TOKEN_TYPE assertType, bool expectEOF = false) {
		return curToken(expectEOF).isType(assertType);
	}
	void advanceToken(bool expectEOF = false) {
		tokenIdx++;
		while (tokenIdx < tokens.size() && curTokenIs(COMMENT, expectEOF)) tokenIdx++;
	}
	bool throwBadToken(Token badToken) {
		throw badToken;
		return false;
	}

	void printFail(Token& e);
	void printSuccess();

	DatalogProg prog;
};

#endif