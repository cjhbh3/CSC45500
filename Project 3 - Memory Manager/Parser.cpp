#include "Parser.hpp"

#include <iostream>

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
        consumeToken();
        if (nextToken == LPAREN) {
            consumeToken();
            if (nextToken == ID) {
                consumeToken();
                if (nextToken == RPAREN) {
                    // ID LPAREN ID RPAREN
                    consumeToken();
                    return true;
                }
            }
            else if (nextToken == RPAREN) {
                // ID LPAREN RPAREN
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

    consumeToken();

    if (nextToken == LPAREN) {
        consumeToken();
        if (nextToken == NUM_INT) {
            consumeToken();
            if (nextToken == RPAREN) {
                // ID LPAREN NUM_INT RPAREN
                consumeToken();
                return true;
            }
        }
    }

    // ID
    return true;
}