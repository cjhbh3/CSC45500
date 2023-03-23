#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <fstream>
#include <iostream>

#include "Token.hpp"

class Parser {
    private:
        Token nextToken;
        std::ifstream ifile;
    public:
        Parser(const char* fileName)
            :nextToken(), ifile() {ifile.open(fileName); nextToken.get(ifile);}

        void consumeToken() {nextToken.get(ifile);}

        bool prog();
        bool slist();
        bool stmt();
        bool rhs();
};

#endif