/*******************************************
 * File: main.cpp                          *
 * Author: CJ Hess                       *
 * Date: 2/13/2023                           *
 * PURPOSE: driver file for project 1      *
 *******************************************/

#include <iostream>
#include <fstream>

#include "Token.hpp"

#define SUCCESS 1
#define FAIL 0

using namespace std;

int prog(Token t, ifstream &ifile);
int slist(Token t, ifstream &ifile);
int stmt(Token t, ifstream &ifile);
int rhs(Token t, ifstream &ifile);

// Global variable to facilitate a lookahead action
Token nextToken;
int numIndents = 0;



int prog(Token t, ifstream &ifile) {
  // declaration prog | compound
  if (declaration(t, ifile)) {
    if (prog(nextToken, ifile)) {
      // declaration prog
      return SUCCESS;
    }
  }
  else if (compound(t, ifile)) {
    // compound
    return SUCCESS;
  }
  return FAIL;
}

int slist(Token t, ifstream &ifile) {
  // stmt | stmt SEMICOLON stmlist
  if (stmt(t, ifile)) {
    if (nextToken.type() == SEMICOLON) {
      cout << nextToken.value() << endl;
      nextToken.get(ifile);
      if (slist(nextToken, ifile)) {
        return SUCCESS;
      }
    }
    else {
      return SUCCESS;
    }
  }
  return FAIL;
}

int stmt(Token t, ifstream &ifile) {
  if (t.type() == ID) {
    // ID | ID LPAREN exprlist RPAREN | ID ASSIGNOP expr
    indent();
    cout << t.value();
    nextToken = t;
    nextToken.get(ifile);
    if (nextToken.type() == LPAREN || nextToken.type() == ASSIGNOP) {
      if (nextToken.type() == LPAREN) {
        // ID LPAREN exprlist RPAREN
        cout << nextToken.value();
        nextToken.get(ifile);
        if (exprlist(nextToken, ifile)) {
          if (nextToken.type() == RPAREN) {
            cout << nextToken.value();
            nextToken.get(ifile);
            return SUCCESS;
          }
        }
      }
      else {
        // ID ASSIGNOP expr
        cout << " " << nextToken.value() << " ";
        nextToken.get(ifile);
        if (expr(nextToken, ifile)) {
          return SUCCESS;
        }
      }
    }
    return SUCCESS;
  }
  else if (t.type() == IF) {
    // IF expr THEN compound ELSE compound
    indent();
    numIndents++;
    cout << t.value() << " ";
    nextToken = t;
    nextToken.get(ifile);
    if (expr(nextToken, ifile)) {
      if (nextToken.type() == THEN) {
        cout << nextToken.value() << endl;
        nextToken.get(ifile);
        if (compound(nextToken, ifile)) {
          if (nextToken.type() == ELSE) {
            numIndents--;
            indent();
            cout << nextToken.value() << endl;
            nextToken.get(ifile);
            if (compound(nextToken, ifile)) {
              return SUCCESS;
            }
          }
        }
      }
    }
  }
  else if (t.type() == WHILE) {
    // WHILE LPAREN expr RPAREN compound
    indent();
    numIndents++;
    cout << t.value() << " ";
    nextToken = t;
    nextToken.get(ifile);
    if (nextToken.type() == LPAREN) {
      cout << nextToken.value();
      nextToken.get(ifile);
      if (expr(nextToken, ifile)) {
        if (nextToken.type() == RPAREN) {
          cout << nextToken.value() << endl;
          nextToken.get(ifile);
          if (compound(nextToken, ifile)) {
            return SUCCESS;
          }
        }
      }
    }
  }
  else {
    // compound
    if (compound(t, ifile)) {
      return SUCCESS;
    }
  }
  return FAIL;
}

int rhs(Token t, ifstream &ifile) {
  if (t.type() == ID) {
    cout << t.value();
    nextToken = t;
    nextToken.get(ifile);
    if (nextToken.type() == LPAREN) {
      cout << nextToken.value();
      nextToken.get(ifile);
      if (nextToken.type() == NUM_INT) {
        cout << nextToken.value();
        nextToken.get(ifile);
        if (nextToken.type() == RPAREN) {
          cout << nextToken.value() << endl;
          return SUCCESS;
        }
      }

    }
    else {
      return SUCCESS;
    }
  }
  return FAIL;
}


int main(int argc, char *argv[])
{
  // prompt for initial block size for the freeList
  int freeListSize = 0;
  char * inputFileName;
  cout << "Please enter the initial freelist (heap) size: ";
  

  // prompt for input file name
  // open input file
  ifstream ifile();

  // if open was not successful, let user know. 
  if (!ifile) {
    cerr << "ERROR: Could not open file:\"" << argv[1] << "\"" <<endl;
    return -1;
  }

  return 0;
}





