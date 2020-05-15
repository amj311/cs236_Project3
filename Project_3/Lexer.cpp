#include "Lexer.h"

Lexer::Lexer(string& input)
{
	this->input = input;
}

void Lexer::tokenizeInput()
{
	lineNumber = 1;
	tokens.clear();

	do {
		tokens.push_back(findNextToken());
	} while (tokens.back()->getType() != EOF_TYPE);
}

Token* Lexer::findNextToken()
{
	size_t longestRead = 0;

	checkWhiteSpace();
	if (input.length() <= 0) return new Token(EOF_TYPE, "", lineNumber);
	size_t mWinner;
	for (size_t m = 0; m < machines.size(); m++)
	{
		size_t charsRead = machines[m]->Read(input);
		if (charsRead > longestRead) {
			longestRead = charsRead;
			mWinner = m;
		}
	}
	Token* token;
	if (longestRead > 0) token = new Token(machines[mWinner]->type, input.substr(0,longestRead), lineNumber);
	else {
		token = new Token(UNDEFINED, input.substr(0,1), lineNumber);
	}

	for (size_t c = 0; c < (*token).getValue().length(); c++) {
		if (input[c] == '\n') lineNumber++;
	}

	input = input.substr((*token).getValue().length());

	return token;
}

void Lexer::checkWhiteSpace() {
	while (input.length() > 0 && isspace(input.front())) {
		if (input.front() == '\n') lineNumber++;
		input = input.substr(1);
	}
}

vector<Token*> Lexer::getTokenList()
{
	return tokens;
}

void Lexer::printTokens()
{
	for (size_t i = 0; i < tokens.size(); i++)
	{
		cout << tokens[i]->toString() << endl;
	}
	cout << "Total Tokens = " << tokens.size() << endl;
}
