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

struct node {
  int startAdr;
  int endAdr;
  int refCount;
  node * next = nullptr;
};

struct variable {
  int startAdr;
  int endAdr;
  int refCount;
  int name;
};

int prog(Token t, ifstream &ifile);
int slist(Token t, ifstream &ifile);
int stmt(Token t, ifstream &ifile);
int rhs(Token t, ifstream &ifile);

// Global variable to facilitate a lookahead action
Token nextToken;
int numIndents = 0;



int prog(Token t, ifstream &ifile) {
  if (slist(t, ifile)) {
    return SUCCESS;
  }
  return FAIL;
}

int slist(Token t, ifstream &ifile) {
  // stmt SEMICOLON slist | E
  if (stmt(t, ifile)) {
    if (nextToken.type() == SEMICOLON) {
      cout << nextToken.value() << endl;
      nextToken.get(ifile);
      if (slist(nextToken, ifile)) {
        return SUCCESS;
      }
    }
  }
  return FAIL;
}

int stmt(Token t, ifstream &ifile) {
  /*
    ID LPAREN ID RPAREN
    ID LPRAREN RPAREN
    ID ASSIGNOP <rhs>
  */
  if (t.type() == ID) {
    cout << t.value();
    nextToken = t;
    nextToken.get(ifile);
    if (nextToken.type() == LPAREN) {
      cout << nextToken.value(); 
      nextToken.get(ifile);
      if (nextToken.type() == ID) {
        cout << nextToken.value();
        nextToken.get(ifile);
        if (nextToken.type() == RPAREN) {
          cout << nextToken.value();
          nextToken.get(ifile);
          return SUCCESS;
        }
      }
      else if (nextToken.type() == RPAREN) {
        cout << nextToken.value();
        nextToken.get(ifile);
        return SUCCESS;
      }
    }
    else if (nextToken.type() == ASSIGNOP) {
      cout << nextToken.value();
      nextToken.get(ifile);
      if (rhs(nextToken, ifile)) {
        return SUCCESS;
      }
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
      nextToken.get(ifile);
      return SUCCESS;
    }
  }
  return FAIL;
}

/*
  Looks like we will need structs/classes to implement the freelist and hold on to variables
*/

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





