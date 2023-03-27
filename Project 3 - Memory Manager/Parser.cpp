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
                    int index = checkVar(varName);
                    if (index != -1) {
                        variables.erase(variables.begin() + index);
                    }
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
                int index = checkVar(varName);
                if (index == -1) {
                    variables.push_back(newVar);
                    cout << variables.size() << endl;
                }
                else {
                    variables.at(index) = newVar;
                    cout << variables.size() << endl;
                }
                consumeToken();
                return true;
            }
        }
    }

    // ID
    // varName = otherVarName
    cout << "varName = otherVarName" << endl;
    int indexExisting = checkVar(varName);
    int indexReference = checkVar(otherVarName);
    if (indexExisting == -1) {
        variables.at(indexReference).setRefCount(variables.at(indexReference).getRefCount() + 1);
        Variable newVar(varName, variables.at(indexReference));
        variables.push_back(newVar);
    }
    else {
        variables.at(indexExisting).reference(variables.at(indexReference));
    }
    
    return true;
}

void Parser::dump() {
    cout << "Variables: " << endl;
    for (int i = 0; i < variables.size(); i++) {
        cout << variables.at(i).getVarName() << ":" << variables.at(i).getStartAddr() << 
        "(" <<variables.at(i).getBlockSize() << ")"
        << " [" << variables.at(i).getRefCount() << "]" << endl;
    }
    cout << endl;
}

int Parser::checkVar(std::string name) {
    // Checks if varName is already present in the variables list
    // Return index if present, -1 if not
    int index = -1;

    for (int i = 0; i < variables.size(); i++) {
        if (variables.at(i).getVarName() == name) {
            index = i;
        }
    }

    return index;
}