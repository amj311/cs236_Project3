#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>
#include <string>
//#include "Tokenizer.h"
#include "Token.h"
#include "Automaton.h"
#include "FSA_Collection.h"
using namespace std;

class Lexer
{
public:
	Lexer(string& input); 
	~Lexer() {
		for (size_t m = 0; m < machines.size(); m++) {
			delete machines[m];
		}

		for (size_t t = 0; t < tokens.size(); t++) {
			delete tokens[t];
		}
	}

	void tokenizeInput();
	vector<Token*> getTokenList();
	void printTokens();

	Token* findNextToken();

private:
	string input;
	int lineNumber = 1;
	//Tokenizer tokenizer;
	vector<Automaton*> machines = {
		new FSA_COMMA(),
		new FSA_PERIOD(),
		new FSA_Q_MARK(),
		new FSA_LEFT_PAREN(),
		new FSA_RIGHT_PAREN(),
		new FSA_COLON(),
		new FSA_MULTIPLY(),
		new FSA_ADD(),
		new FSA_COMMENT_LONG(),
		new FSA_COMMENT(),
		new FSA_STRING(),
		new FSA_SCHEMES(),
		new FSA_FACTS(),
		new FSA_RULES(),
		new FSA_QUERIES(),
		new FSA_ID(),
	};
	vector<Token*> tokens;

	void checkWhiteSpace();
};

#endif
