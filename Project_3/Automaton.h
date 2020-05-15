#ifndef AUTOMATON_H
#define AUTOMATON_H

#include "TokenTypes.h"
#include <iostream>
#include <string>
using namespace std;

class Automaton
{
public:
	Automaton() {};
	virtual ~Automaton() = default;

	virtual size_t Read(string& input) = 0;
	TOKEN_TYPE type = UNDEFINED;
	string lastRead();

protected:
	size_t readIdx = 0;


	char curCharOf(string& input)
	{
		return input[readIdx];
	}

	char nextCharOf(string& input)
	{
		return input[readIdx+1];
	}

	void advanceChar()
	{
		readIdx++;
	}
};

#endif