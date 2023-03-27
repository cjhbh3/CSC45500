#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <fstream>
#include <iostream>
#include <vector>

#include "Token.hpp"
#include "Variable.hpp"



class Parser {
    private:
        Token nextToken;
        std::ifstream ifile;
        std::vector<Variable> variables;

        std::string varName;
        std::string otherVarName;
        int blockSize;
    public:
        Parser(const char* fileName)
            :nextToken(), ifile() {ifile.open(fileName); nextToken.get(ifile);}

        void consumeToken() {nextToken.get(ifile);}

        bool prog();
        bool slist();
        bool stmt();
        bool rhs();

        void dump();
        int checkVar(std::string name);
};

#endif