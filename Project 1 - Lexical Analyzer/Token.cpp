/*******************************************
 * File: Token.cpp                         *
 * Author: S. Blythe                       *
 * Date: 12/2022                           *
 * PURPOSE: implementation for Token       *
 *******************************************/

#include "Token.hpp"

#include <fstream>
#include <iomanip>

using namespace std;

// the promised global for string equivalents of TokenType enumeration
string TokStr[]=
{ "ERROR", "EOF_TOK", "NUM_INT", "NUM_REAL", "ADDOP", "MULOP", "ID", "RELOP", "ASSIGNOP", "LPAREN", "RPAREN",  "SEMICOLON",  "LBRACK", "RBRACK", "COMMA", "AND", "OR", "INTEGER", "FLOAT", "WHILE", "IF", "THEN", "ELSE", "VOID", "BEGIN", "END"};

// This is a "list" of the keywords. Note that they are in the same order
//   as found on the TokenType enumaration. 
static string reserved[]={"int" , "float", "while", "if", "then", "else", "void", "begin", "end" };



/******************************************************
 *  just prints out the info describing this Token    *
 *    to specified stream                             *
 *                                                    *
 *   os  - the stream to add the Token to             *
 *                                                    *
 *   returns: the updated stream                      *
 ******************************************************/
ostream&
Token::print(ostream& os) const
{
  os
     << "{ Type:"   << left << setw(10) << TokStr[_type] 
     << " Value:"   << left << setw(10) << _value
     << " Line Number:" << _line_num
     << " }";
  return os;
}

/******************************************************
 *  Fills in information about this Token by reading  *
 *    it from specified input stream                  *
 *                                                    *
 *   is  - the stream to read the Token from          *
 *                                                    *
 *   returns: nothing                                 *
 *                                                    *
 *     **** YOU MUST CODE THIS !!!!!! ****            *
 ******************************************************/

static int ** DFA= nullptr;

void Token::get(istream &is)
{
  // you must write this code !!!!
  if (DFA == nullptr) {
    //DFA = new int*[COMMA];
  }


  _value="";
  char ch;

  // skipWS()
  if(!is) 
    return;
  
  
  int curr=0;
  int prev=-1;

  while (curr != -1) {

    ch = is.get();
    if (ch != '#') {
      prev = curr;
      curr = DFA[curr][(int)ch];

      if (curr != -1)
        _value+=ch;
    }
    else {
      is.ignore(numeric_limits<streamsize>::max(), '\n');
    }

  }

  if (is) {
    is.putback(ch);
  }

  if (is.eof()) {
    prev = 1;
  }

  _type = static_cast<TokenType>(prev);
  return;
}
