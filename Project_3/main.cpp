#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include "Lexer.h"
#include "Parser.h"
#include "DatalogProg.h"
#include "Interpretor.h"

int main(int argc, char** argv)
{
    ifstream ifs(argv[1]);
    string content((istreambuf_iterator<char>(ifs)),
        (istreambuf_iterator<char>()));

    Lexer lexer(content);
    lexer.tokenizeInput();

    Parser parser(lexer.getTokenList());
    parser.run();

    DatalogProg prog = parser.getParsedProgram();

    Interpretor interp = Interpretor();
    interp.interpretProgram(prog);

    return 0;
}