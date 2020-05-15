#ifndef FSA_COLLECTION_H
#define FSA_COLLECTION_H

#include "Automaton.h"


class FSA_COMMA : public virtual Automaton
{
public:
	FSA_COMMA() { type = COMMA; };

	size_t Read(string& input) {
		readIdx = 0;
		if (curCharOf(input) == ',') {
			advanceChar();
		}
		return readIdx;
	}
};

class FSA_PERIOD : public virtual Automaton
{
public:
	FSA_PERIOD() { type = PERIOD; };

	size_t Read(string& input) {
		readIdx = 0;
		if (curCharOf(input) == '.') {
			advanceChar();
		}
		return readIdx;
	}
};

class FSA_Q_MARK : public virtual Automaton
{
public:
	FSA_Q_MARK() { type = Q_MARK; };

	size_t Read(string& input) {
		readIdx = 0;
		if (curCharOf(input) == '?') {
			advanceChar();
		}
		return readIdx;
	}
};

class FSA_MULTIPLY : public virtual Automaton
{
public:
	FSA_MULTIPLY() { type = MULTIPLY; };

	size_t Read(string& input) {
		readIdx = 0;
		if (curCharOf(input) == '*') {
			advanceChar();
		}
		return readIdx;
	}
};

class FSA_ADD : public virtual Automaton
{
public:
	FSA_ADD() { type = ADD; };

	size_t Read(string& input) {
		readIdx = 0;
		if (curCharOf(input) == '+') {
			advanceChar();
		}
		return readIdx;
	}
};

class FSA_LEFT_PAREN : public virtual Automaton
{
public:
	FSA_LEFT_PAREN() { type = LEFT_PAREN; };

	size_t Read(string& input) {
		readIdx = 0;
		if (curCharOf(input) == '(') {
			advanceChar();
		}
		return readIdx;
	}
};

class FSA_RIGHT_PAREN : public virtual Automaton
{
public:
	FSA_RIGHT_PAREN() { type = RIGHT_PAREN; };

	size_t Read(string& input) {
		readIdx = 0;
		if (curCharOf(input) == ')') {
			advanceChar();
		}
		return readIdx;
	}
};

class FSA_COLON : public virtual Automaton
{
public:
	FSA_COLON() { type = COLON; };

	size_t Read(string& input) {
		readIdx = 0;
		type = COLON;

		if (curCharOf(input) == ':') {
			advanceChar();
			if (curCharOf(input) == '-') {
				type = COLON_DASH;
				advanceChar();
			}
		}
		return readIdx;
	}
};



class FSA_COMMENT : public virtual Automaton
{
public:
	FSA_COMMENT() { type = COMMENT; };

	size_t Read(string& input) {
		readIdx = 0;

		if (curCharOf(input) == '#' && nextCharOf(input) != '|') {
			while (readIdx < input.length()) {
				if (curCharOf(input) == '\n') break;
				else advanceChar();
			}
		}
		return readIdx;
	}
};

class FSA_COMMENT_LONG : public virtual Automaton
{
public:
	FSA_COMMENT_LONG() { type = COMMENT; };

	size_t Read(string& input) {
		readIdx = 0;
		type = COMMENT;

		if (curCharOf(input) == '#') {
			advanceChar();
		}
		else return readIdx;

		if (curCharOf(input) == '|') {
			advanceChar();

			while (readIdx < input.length()) {
				if (curCharOf(input) == '|') {
					advanceChar();
					if (curCharOf(input) == '#') {
						advanceChar();
						return readIdx;
					}
				}
				else advanceChar();
			}
			type = UNDEFINED;
		}
		return readIdx;
	}
};


class FSA_STRING : public virtual Automaton
{
public:
	FSA_STRING() { type = STRING; };

	size_t Read(string& input) {
		readIdx = 0;
		type = STRING;

		if (curCharOf(input) == '\'') {
			
			while (readIdx < input.length()) {
				advanceChar();


				if (curCharOf(input) == '\'' && nextCharOf(input) == '\'') {
					advanceChar();
					continue;
				}

				if (curCharOf(input) == '\'' && nextCharOf(input) != '\'') {
					advanceChar();
					return readIdx;
				}
			}
		}
		type = UNDEFINED;
		return readIdx;
	}
};





class FSA_SCHEMES : public virtual Automaton
{
public:
	FSA_SCHEMES() { type = SCHEMES; };

	size_t Read(string& input) {
		readIdx = 0;
		string token = "Schemes";
		
		while (readIdx < token.length()) {
			if (curCharOf(input) == curCharOf(token)) advanceChar();
			else return 0;
		}

		return readIdx;
	}
};

class FSA_FACTS : public virtual Automaton
{
public:
	FSA_FACTS() { type = FACTS; };

	size_t Read(string& input) {
		readIdx = 0;
		string token = "Facts";

		while (readIdx < token.length()) {
			if (curCharOf(input) == curCharOf(token)) advanceChar();
			else return 0;
		}

		return readIdx;
	}
};

class FSA_RULES : public virtual Automaton
{
public:
	FSA_RULES() { type = RULES; };

	size_t Read(string& input) {
		readIdx = 0;
		string token = "Rules";

		while (readIdx < token.length()) {
			if (curCharOf(input) == curCharOf(token)) advanceChar();
			else return 0;
		}

		return readIdx;
	}
};

class FSA_QUERIES : public virtual Automaton
{
public:
	FSA_QUERIES() { type = QUERIES; };

	size_t Read(string& input) {
		readIdx = 0;
		string token = "Queries";

		while (readIdx < token.length()) {
			if (curCharOf(input) == curCharOf(token)) advanceChar();
			else return 0;
		}

		return readIdx;
	}
};




class FSA_ID : public virtual Automaton
{
public:
	FSA_ID() { type = ID; };
	~FSA_ID() {};

	size_t Read(string& input) {
		readIdx = 0;
		
		if (!isalpha(curCharOf(input))) return readIdx;

		while (readIdx < input.length()) {
			//if (!isalpha(curCharOf(input)) || isspace(curCharOf(input))) return readIdx;
			if (isspace(curCharOf(input))) return readIdx;

			switch (curCharOf(input))
			{
				// Special Characters
			case ',': return readIdx;
			case '.': return readIdx;
			case '?': return readIdx;
			case '(': return readIdx;
			case ')': return readIdx;
			case ':': return readIdx;
			case '*': return readIdx;
			case '+': return readIdx;
			case '\'': return readIdx;
			case '#': return readIdx;
			default:
				advanceChar();
			}
		}

		return readIdx;
	}
};





#endif