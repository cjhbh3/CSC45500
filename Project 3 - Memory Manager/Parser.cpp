#include "Parser.hpp"

#include <iostream>
#include <stdlib.h>

using namespace std;

bool Parser::prog() {
    return slist();
}

bool Parser::slist() {
    if (stmt()) {
        if (nextToken == SEMICOLON) {
            consumeToken();
            return slist();
        }
        else {
            // We have an issue
            return false;
        }
    }
    else {
        // Epsilon
        return true;
    }
}

bool Parser::stmt() {
    /*
        ID LPAREN ID RPAREN
        ID LPAREN RPAREN
        ID ASSIGNOP <rhs>
    */
    if (nextToken == ID) {
        // We will need this
        string idName = nextToken.value();
        varName = nextToken.value();
        consumeToken();
        if (nextToken == LPAREN) {
            consumeToken();
            if (nextToken == ID) {
                varName = nextToken.value();
                consumeToken();
                if (nextToken == RPAREN) {
                    // ID LPAREN ID RPAREN
                    // free(varName)
                    cout << "free()" << endl;
                    consumeToken();
                    return true;
                }
            }
            else if (nextToken == RPAREN) {
                // ID LPAREN RPAREN
                // dump() or compress()
                if (idName == "dump") {
                    cout << "dump()" << endl;
                    dump();
                }
                else if (idName == "compress") {
                    cout << "compress()" << endl;
                }
                consumeToken();
                return true;
            }
        }
        else if (nextToken == ASSIGNOP) {
            // ID ASSIGNOP rhs
            consumeToken();
            return rhs();
        }
    }
    return false;
}

bool Parser::rhs() {
    /*
        ID LPRAREN NUM_INT RPAREN
        ID
    */
    if (nextToken != ID) {
        // rhs can't start with anything except ID
        return false;
    }

    otherVarName = nextToken.value();
    consumeToken();

    if (nextToken == LPAREN) {
        consumeToken();
        if (nextToken == NUM_INT) {
            blockSize = atoi(nextToken.value().c_str());
            consumeToken();
            if (nextToken == RPAREN) {
                // ID LPAREN NUM_INT RPAREN
                // varName = alloc(integerAmount)
                cout << "varName = alloc(int)" << endl;
                Variable newVar(0, 0 + blockSize, 1, blockSize, varName);
                variables.push_back(newVar);
                cout << variables.size() << endl;
                consumeToken();
                return true;
            }
        }
    }

    // ID
    // varName = otherVarName
    cout << "varName = otherVarName" << endl;
    return true;
}

void Parser::dump() {
    for (int i = 0; i < variables.size(); i++) {
        cout << variables.at(i).getVarName() << ":" << variables.at(i).getBlockSize() 
        << " [" << variables.at(i).getRefCount() << "]" << endl;
    }
}