// Name: CJ Hess
// Date: 1/19/2023
// Filename: Token.cpp

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

static int **DFA=NULL;

int skipWS(istream &is) {
  char ch = is.get();
  int newLines = 0;
  while (ch == ' ' || ch == '\n' || ch == '\r') {
    if (ch == '\n') {
      newLines++;
    }
    ch = is.get();
  }
  if (is)
    is.putback(ch);

  return newLines;
}

void cleanUp() {
  delete[] DFA;
  DFA = NULL;
  return;
}

void Token::get(istream &is)
{
  // you must write this code !!!!
  if (DFA == NULL) {

    // OR is 16 + 4 more spots (I use some throw away numbers to represent non-terminal states)
    DFA = new int*[20];
    
    // NUM_INT to OR
    // Adding 3 to account for non-terminate states 3, 12, 14
    /*
        3 = 17
        12 = 18
        14 = 19
    */
    for (int state = 0; state <= 19; state++) {
      DFA[state] = new int[256];

      // Set every spot to an Error as default
      for (int ch = 0; ch < 256; ch++) {
        DFA[state][ch] = -1;
      }
    }

    for (char ch = '0'; ch <= '9'; ch++) {
      DFA[0][(int) ch] = NUM_INT;
    }

    // For uppercase letters
    for (char ch = 'A'; ch <= 'Z'; ch++) {
      DFA[0][(int) ch] = ID;
    }

    // For lowercase letters
    for (char ch = 'a'; ch <= 'z'; ch++) {
      DFA[0][(int) ch] = ID;
    }
    
    DFA[0][(int) '+'] = ADDOP;
    DFA[0][(int) '-'] = ADDOP;
    DFA[0][(int) '/'] = MULOP;
    DFA[0][(int) '*'] = MULOP;
    DFA[0][(int) '<'] = RELOP;
    DFA[0][(int) '>'] = RELOP;
    DFA[0][(int) '='] = ASSIGNOP;
    DFA[0][(int) '('] = LPAREN;
    DFA[0][(int) ')'] = RPAREN;
    DFA[0][(int) ';'] = SEMICOLON;
    DFA[0][(int) '['] = LBRACK;
    DFA[0][(int) ']'] = RBRACK;
    DFA[0][(int) ','] = COMMA;

    // Special cases where they are non-terminate
    DFA[0][(int) '&'] = 18;
    DFA[18][(int) '&'] = AND;
    DFA[0][(int) '|'] = 19;
    DFA[19][(int) '|'] = OR;

    for (char ch = '0'; ch <= '9'; ch++) {
      DFA[NUM_INT][(int) ch] = NUM_INT;
    }

    // Make sure that if we are in an ID, we move to an ID with another letter
    // Then if we are in an ID and see a number, we can move to ID and add the number to the string
    // For uppercase letters
    for (char ch = 'A'; ch <= 'Z'; ch++) {
      DFA[ID][(int) ch] = ID;
    }

    // For lowercase letters
    for (char ch = 'a'; ch <= 'z'; ch++) {
      DFA[ID][(int) ch] = ID;
    }

    for (char ch = '0'; ch <= '9'; ch++) {
      DFA[ID][(int) ch] = ID;
    }

    // If we are in a NUM_INT and see a "." -> NUM_REAL
    // Now theoretically, it shouldn't be NUM_REAL right away because that is only if a digit follows the "."
    DFA[NUM_INT][(int) '.'] = 17;
    
    for (char ch = '0'; ch <= '9'; ch++) {
      // 17 being the state that connects #. to another digit (Ex: 3. -> 3.1)
      DFA[17][(int) ch] = NUM_REAL;
    }

    DFA[RELOP][(int) '='] = RELOP;
    DFA[ASSIGNOP][(int) '='] = RELOP;
  }

  // If the line number hasn't be init, init it to 1
  if (_line_num == 0)
    _line_num = 1;

  _value="";
  char ch;

  int linesSkipped = skipWS(is);
  _line_num += linesSkipped;
  if (!is) {
    _value = "";
    _type = EOF_TOK;
    //cleanUp();
    return;
  }
  
  int curr=0;
  int prev=-1;

  while (curr != -1) {

    ch = is.get();

    if (ch == '\n') {
      int linesSkipped = skipWS(is);
      _line_num += linesSkipped;
      if (!is) {
        _value = "";
        _type = EOF_TOK;
        cleanUp();
        return;
      }
    }
    else if (ch != '#') {
      
      prev = curr;
      curr = DFA[curr][(int) ch];
      if (curr != -1)
        _value+=ch;
    }
    else {
      is.ignore(100000, '\n');
      _line_num++;
      int linesSkipped = skipWS(is);
      _line_num += linesSkipped;
      if (!is) {
        _value = "";
        _type = EOF_TOK;
        cleanUp();
        return;
      }
    }
  }
  if (is && ch != '#') {
    is.putback(ch);
  }
  
  // Check if the ID is any reserved ID
  if (prev == ID) {
    if (_value == "void") {
      prev = VOID;
    }
    else if (_value == "int") {
      prev = INTEGER;
    }
    else if (_value == "float") {
      prev = FLOAT;
    }
    else if (_value == "begin") {
      prev = BEGIN;
    }
    else if (_value == "end") {
      prev = END;
    }
    else if (_value == "if") {
      prev = IF;
    }
    else if (_value == "then") {
      prev = THEN;
    }
    else if (_value == "else") {
      prev = ELSE;
    }
    else if (_value == "while") {
      prev = WHILE;
    }
  }
  _type = static_cast<TokenType>(prev);
  return;
}
