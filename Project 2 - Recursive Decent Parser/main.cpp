/*******************************************
 * File: main.cpp                          *
 * Author: S. Blythe                       *
 * Date: 12/2022                           *
 * PURPOSE: driver file for project 1      *
 *******************************************/

/**********
 **********  DO NOT MODIFY THIS FILE!!!!!
 **********/

#include <iostream>
#include <fstream>

#include "Token.hpp"

#define SUCCESS 1
#define FAIL 0

using namespace std;

void stmt(Token t, ifstream ifile) {
  Token nextToken = t;
  nextToken.get(ifile);
  if (t.type() == ID) {
    cout << t.value() << " ";
    t.get(ifile);
    if (t.type() == LPAREN) {
      cout << t.value() << " ";
      t.get(ifile);
      exprlist(t, ifile);
      if (t.type() == RPAREN) {
        cout << t.value() << " ";
      }
    }
    else {
      if (t.type() == ASSIGNOP) {
        cout << t.value() << " ";
        t.get(ifile);
        expr(t, ifile);
      }
    }
  }
  else if (t.type() == IF) {
    cout << t.value() << " ";
    t.get(ifile);
    expr(t, ifile);
    if (t.type() == THEN) {
      cout << t.value() << endl;
      t.get(ifile);
      compound(t, ifile);
      if (t.type() == ELSE) {
        cout << t.value() << endl;
        compound(t, ifile);
      }
    }
  }
  else if (t.type() == WHILE) {

  }
  else {
    compound(t, ifile);
  }
}

void exprlist(Token t, ifstream ifile) {
  Token nextToken = t;
  nextToken.get(ifile);
  if (nextToken.type() == COMMA) {
    expr(t, ifile);
    cout << nextToken.value() << " ";
    nextToken.get(ifile);
    exprlist(nextToken, ifile);
  }
  else {
    expr();
  }
}

void expr(Token t, ifstream ifile) {
  Token nextToken = t;
  nextToken.get(ifile);
  if (nextToken.type() == RELOP) {
    simpexpr(t, ifile);
    cout << nextToken.value() << " ";
    nextToken.get(ifile);
    simpexpr(nextToken, ifile);
  }
  else {
    simpexpr(t, ifile);
  }
}

void simpexpr(Token t, ifstream ifile) {
  Token nextToken = t;
  nextToken.get(ifile);
  if (nextToken.type() == ADDOP) {
    expr(t, ifile);
    cout << nextToken.value() << " ";
    nextToken.get(ifile);
    simpexpr(nextToken, ifile);
  }
  else {
    term(t, ifile);
  }
}

void term(Token t, ifstream ifile) {
  Token nextToken = t;
  nextToken.get(ifile);
  if (nextToken.type() == MULOP) {
    factor(t, ifile);
    cout << nextToken.value() << " ";
    nextToken.get(ifile);
    term(nextToken, ifile);
  }
  else {
    factor(t, ifile);
  }
}

void factor(Token t, ifstream ifile) {
  /*
    This should work, but I will more than likely have to 
    add a SUCCESS/FAIL return to ensure that we 
    can properly spot if the program is valid
  */
  if (t.type() == NUM_REAL || t.type() == NUM_INT) {
    cout << t.value() << " ";
  }
  else {
    if (t.type() == ID) {
      Token currentToken = t;
      Token nextToken = t.get(ifile);
      if (nextToken.type() == LPAREN) {
        cout << nextToken.value() << " ";
        nextToken.get(ifile);
        exprlist(nextToken, ifile); // This would print the exprlist if its there
        if (nextToken.type() == RPAREN) {
          cout << nextToken.value() << " ";
          // At this point it would be something like
          // a ( b, c, d )
        }
      }
      else {
        cout << currentToken.value() << " ";
      }
    }
    else {
      if (t.type() == LPAREN) {
        cout << t.value() << " ";
        t.get(ifile);
        expr(); // Would print the expr if exist
        if (t.type() == RPAREN) {
          cout << t.value() << " ";
        }
      }
    }
  }
}

int main(int argc, char *argv[])
{
  // check to make sure an input file was specified. 
  if (argc!=2) {
    cerr << "USAGE: ./tokenify <file>" << endl;
    return -1;
  }

  // open input file
  ifstream ifile(argv[1]);

  // if open was not successful, let user know. 
  if (!ifile) {
    cerr << "ERROR: Could not open file:\"" << argv[1] << "\"" <<endl;
    return -1;
  }


  // *VERY* simple main function. Just repeatedly call Token's get method!
  Token tok;

  // (try to) get the first token
  tok.get(ifile);

  // did the last get call say there were more tokens? 
  while( tok.type()!=EOF_TOK ) {
    if (tok.type()!=ERROR) {
	  // print out the successfully read Token
	    cout << "Resulting token = " << tok << endl;
	  }
    else { // tok.type()==ERROR 
      cout << "Syntax error detected on line " << tok.lineNumber() << endl;
	  }
    // (try to) get the next token 
    tok.get(ifile);
  }

  return 0;
}





