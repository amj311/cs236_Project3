#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <vector>
#include "TokenTypes.h"
using namespace std;

class Token
{
private:
	TOKEN_TYPE type = EOF_TYPE;
	string value = "";
	int line = 0;
public:
	Token() {};
	Token(TOKEN_TYPE type, string value, int line);

	// GETTERS
	TOKEN_TYPE getType();
	string getValue();
	int getLine();
	bool isType(TOKEN_TYPE);

	string toString();

	vector<string> TOKEN_STRINGS = {
		"COMMA",
		"PERIOD",
		"Q_MARK",
		"LEFT_PAREN",
		"RIGHT_PAREN",
		"COLON",
		"COLON_DASH",
		"MULTIPLY",
		"ADD",
		"SCHEMES",
		"FACTS",
		"RULES",
		"QUERIES",
		"ID",
		"STRING",
		"COMMENT",
		"UNDEFINED",
		"EOF"
	};
};

#endif // !TOKEN_H