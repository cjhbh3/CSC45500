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

// Helper function for getting proper spacing.
void indent();

int program(Token t, ifstream &ifile);
int declaration(Token t, ifstream &ifile);
int idlist(Token t, ifstream &ifile);
int type(Token t, ifstream &ifile);
int compound(Token t, ifstream &ifile);
int stmtlist(Token t, ifstream &ifile);
int stmt(Token t, ifstream &ifile);
int exprlist(Token t, ifstream &ifile);
int expr(Token t, ifstream &ifile);
int simpexpr(Token t, ifstream &ifile);
int term(Token t, ifstream &ifile);
int factor(Token t, ifstream &ifile);

// Global variable to facilitate a lookahead action
Token nextToken;
int numIndents = 0;

void indent() {
  for (int i = 0; i < numIndents; i++) {
    cout << "   ";
  }
}

int program(Token t, ifstream &ifile) {
  // declaration program | compound
  if (declaration(t, ifile)) {
    if (program(nextToken, ifile)) {
      // declaration program
      return SUCCESS;
    }
  }
  else if (compound(t, ifile)) {
    // compound
    return SUCCESS;
  }
  return FAIL;
}

int declaration(Token t, ifstream &ifile) {
  // type idlist SEMICOLON
  if (type(t, ifile)) {
    nextToken = t;
    nextToken.get(ifile);
    if (idlist(nextToken, ifile)) {
      if (nextToken.type() == SEMICOLON) {
        cout << nextToken.value() << endl;
        nextToken.get(ifile);
        return SUCCESS;
      }
    }
  }
  return FAIL;
}

int idlist(Token t, ifstream &ifile) {
  // ID | ID COMMA idlist
  if (t.type() == ID) {
    cout << t.value();
    nextToken = t;
    nextToken.get(ifile);
    if (nextToken.type() == COMMA) {
      cout << nextToken.value() << " ";
      nextToken.get(ifile);
      if (idlist(nextToken, ifile)) {
        return SUCCESS;
      }
    }
    else {
      return SUCCESS;
    }
  }
  return FAIL;
}

int type(Token t, ifstream &ifile) {
  // INTEGER | FLOAT | VOID
  // Initally I had one massive OR statement for all three, 
  // but it would result in any type being printed as int for some reason
  if (t.type() == INTEGER) {
    cout << t.value() << " ";
    return SUCCESS;
  }
  else if (t.type() == FLOAT) {
    cout << t.value() << " ";
    return SUCCESS;
  }
  else if (t.type() == VOID) {
    cout << t.value() << " ";
    return SUCCESS;
  }
  return FAIL;
}

int compound(Token t, ifstream &ifile) {
  // BEGIN stmtlist END
  if (t.type() == BEGIN) {
    indent();
    numIndents++;
    cout << t.value() << endl;
    nextToken = t;
    nextToken.get(ifile);
    if (stmtlist(nextToken, ifile)) {
      if (nextToken.type() == END) {
        Token end = nextToken;
        nextToken.get(ifile);
        // When actually working on the code beauty I initially would put the indents first, the put a new line
        // Pretty dumbfounded when I figured that out.
        cout << endl;
        numIndents--;
        indent();
        cout << end.value();
        if (nextToken.type() != SEMICOLON) {
          cout << endl;
        }
        return SUCCESS;
      }
    }
  }
  return FAIL;
}

int stmtlist(Token t, ifstream &ifile) {
  // stmt | stmt SEMICOLON stmlist
  if (stmt(t, ifile)) {
    if (nextToken.type() == SEMICOLON) {
      cout << nextToken.value() << endl;
      nextToken.get(ifile);
      if (stmtlist(nextToken, ifile)) {
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

int exprlist(Token t, ifstream &ifile) {
  if (expr(t, ifile)) {
    if (nextToken.type() == COMMA) {
      cout << nextToken.value() << " ";
      nextToken.get(ifile);
      if (exprlist(nextToken, ifile)) {
        return SUCCESS;
      }
    }
    else {
      return SUCCESS;
    }
  }
  return FAIL;
}

int expr(Token t, ifstream &ifile) {
  if (simpexpr(t, ifile)) {
    if (nextToken.type() == RELOP) {
      cout << " " << nextToken.value() << " ";
      nextToken.get(ifile);
      if (simpexpr(nextToken, ifile)) {
        // this condition is strange, cause the project doc
        // says simexpr, but when testing it doesn't do it correctly if it is simpexpr
        // When I fixed exprlist and changed this back to simpexpr, it seems to work correctly now
        return SUCCESS;
      }
    }
    else {
      return SUCCESS;
    }
  }
  return FAIL;
}

int simpexpr(Token t, ifstream &ifile) {
  if (term(t, ifile)) {
    if (nextToken.type() == ADDOP) {
      cout << " " << nextToken.value() << " ";
      nextToken.get(ifile);
      if (simpexpr(nextToken, ifile)) {
        return SUCCESS;
      }
    }
    else {
      return SUCCESS;
    }
  }
  return FAIL;
}

int term(Token t, ifstream &ifile) {
  if (factor(t, ifile)) {
    if (nextToken.type() == MULOP) {
      cout << " " << nextToken.value() << " ";
      nextToken.get(ifile);
      if (term(nextToken, ifile)) {
        return SUCCESS;
      }
    }
    else {
      return SUCCESS;
    }
  } 
  return FAIL;
}

int factor(Token t, ifstream &ifile) {
  /*
    This should work, but I will more than likely have to 
    add a SUCCESS/FAIL return to ensure that we 
    can properly spot if the program is valid
  */
  if (t.type() == ID) {
    cout << t.value();
    nextToken = t;
    nextToken.get(ifile);
    if (nextToken.type() == LPAREN) {
      // ID LPAREN exprlist RPAREN
      cout << nextToken.value();
      nextToken.get(ifile);
      if (exprlist(nextToken, ifile)) {
        if (nextToken.type() == RPAREN) {
          cout << nextToken.value() << " ";
          nextToken.get(ifile);
          return SUCCESS;
        }
      }
    }
    else {
      // ID
      return SUCCESS;
    }
  }
  else if (t.type() == NUM_REAL || t.type() == NUM_INT) {
    // NUM_REAL | NUM_INT
    cout << t.value();
    nextToken.get(ifile);
    return SUCCESS;
  }
  else if (t.type() == LPAREN) {
    // LPAREN expr RPAREN
    cout << t.value();
    nextToken = t;
    nextToken.get(ifile);
    if (expr(nextToken, ifile)) {
      // nextToken would *theoretically have the ')' after expr finishes
      if (nextToken.type() == RPAREN) {
        cout << nextToken.value() << " ";
        nextToken.get(ifile);
        return SUCCESS;
      }
    }
  }
  
  return FAIL;
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

  cout << "Successful code =====================" << endl;

  while( tok.type()!=EOF_TOK ) {
    if (tok.type()!=ERROR) {
      program(tok, ifile);
	  }
    else { // tok.type()==ERROR 
      cout << "Syntax error detected on line " << tok.lineNumber() << endl;
	  }
    // (try to) get the next token 
    tok.get(ifile);
  }

  return 0;
}





