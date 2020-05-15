#ifndef PARAM_H
#define PARAM_H

#include <string>
using namespace std;

class Parameter
{
public:
	Parameter(string value) : valueStr(value) {
		if (value[0] == '\'') isString = true;
	}

	bool isSTRING() { return isString; }
	bool isID() { return !isString; }

	string value() {
		return valueStr;
	}

private:
	string valueStr;
	bool isString = false;
};

#endif