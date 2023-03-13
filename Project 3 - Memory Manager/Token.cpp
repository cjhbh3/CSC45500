/*******************************************
 * File: Token.cpp                         *
 * Author: S. Blythe                       *
 * Date: 12/2022                           *
 * PURPOSE: implementation for Token       *
 *******************************************/
#include "Token.hpp"

#include <fstream>
#include <iomanip>

// DFA has 20 states. 
#define MAXSTATES 20

using namespace std;

// keep track of current line number. 
static int __linenum=1;

// the promised global for string equivalents of TokenType enumeration
string TokStr[]=
{ "ERROR", "EOF_TOK", "NUM_INT", "NUM_REAL", "ADDOP", "MULOP", "ID", "RELOP", "ASSIGNOP", "LPAREN", "RPAREN",  "SEMICOLON",  "LBRACK", "RBRACK", "COMMA", "AND", "OR", "INTEGER", "FLOAT", "WHILE", "IF", "THEN", "ELSE", "VOID", "BEGIN", "END"};

// This is a "list" of the keywords. Note that they are in the same order
//   as found on the TokenType enumaration. 
static string reserved[]={"int" , "float", "while", "if", "then", "else", "void", "begin", "end" };


/********************************************
 *  skips any upcoming whitespace in        *
 *     specified stream                     *
 *                                          *
 *  is - the stream to skip whitespace in   *
 *                                          *
 *  returns true if whitespace was skipped, *
 *          false if no whitespace skipped  *
 ********************************************/
bool skip_ws(istream & is)
{
  char ch;
  bool flag=false; // no whitespace skipped yet ...

  // read next character from input stream; as long as it is
  //   whitespace, keep going
  while( isspace(ch=is.get()) )
    {
      flag=true; //we've now seen whitespace

      // if the whitespace is a newline, and 1 to line count
      if (ch=='\n')
	__linenum++;
    }

  // last character read was not whitespace ... so put it back!
  is.unget();
  return flag;
}

/********************************************
 *  skips one comment in input stream       *
 *     but only if that comment exists next *
 *                                          *
 *  is - the stream to skip 1 commen in     *
 *                                          *
 *  returns true if comment was skipped,    *
 *          false if no comment skipped     *
 ********************************************/
bool skip_comm(istream &is)
{
  char ch;

  // get next character from input stream
  ch = is.get();

  // if character is not a #, this is not a comment. Put character back!
  if (ch != '#')
    {
      is.unget();   // put character back. 
      return false;
    }

  // skip remaining characters in comment. 
  while ( (ch=is.get())!='\n' );

  __linenum++; // a comment takes up one line ...

  return true;
}

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
 ******************************************************/
void Token::get(istream &is)
{
  // some "local" variables that need NOT be rebuilt with each call (thus are static). 
  static bool table_built=false;  // have we built the DFA table yet? 
  static int DFA[MAXSTATES][256]; // the DFA table

  // if we haven't built the table yet ...
  if (!table_built)
    {
      int snum; // row index variable for DFA table
      int inp;  // col index variable for DFA table

      // initialize all DFA entires to be invalid. 
      for (snum=0; snum<MAXSTATES; snum++)
	for(inp=0; inp<256; inp++)
	  DFA[snum][inp]=-1;

      // into state 1
      for(inp='A'; inp<='Z'; inp++)
	DFA[0][inp]=DFA[1][inp]=1;
      for(inp='a'; inp<='z'; inp++)
	DFA[0][inp]=DFA[1][inp]=1;
      for(inp='0'; inp<='9'; inp++)
        DFA[1][inp]=1;

      // into state 2
      for(inp='0'; inp<='9'; inp++)
	DFA[0][inp]=DFA[2][inp]=2;


      // into state 3
      DFA[2][(int) '.']=3;
      

      // into state 4
      for(inp='0'; inp<='9'; inp++)
	DFA[3][inp]=DFA[4][inp]=4;


      // into state 5
      DFA[0][(int)'+']=DFA[0][(int)'-']=5;

      // into state 6
      DFA[0][(int)'*']=DFA[0][(int)'/']=6;


      // into state 7
      DFA[0][(int)'<']=7;
      DFA[0][(int)'>']=7;

      // into state 8
      DFA[7][(int)'=']=8;
      DFA[9][(int)'=']=8;
      
      // into state 9
      DFA[0][(int)'=']=9;

      // into state 10
      DFA[0][(int)'(']=10;

      // into state 11
      DFA[0][(int)')']=11;
      
      // into state 12
      DFA[0][(int)'&']=12;

      // into state 13
      DFA[12][(int)'&']=13;
 

      // into state 14
      DFA[0][(int)'|']=14;

      // into state 14
      DFA[0][(int)',']=14;

      // into state 15
      DFA[14][(int)'|']=15;

      // into state 16
      DFA[0][(int)';']=16;

      // into state 17
      DFA[0][(int)'[']=17;

      // into state 18
      DFA[0][(int)']']=18;

      // into state 19
      DFA[0][(int)',']=19;

      table_built=true;
    }

  // this is the beginning of filling in Token values.

  // clear out any current value in lexeme string
  _value.clear();

  // keep skipping whitespace or comments as long as you see them
  while(skip_comm(is) || skip_ws(is) ); /* note "Tricky" semicolon here */

  // if there's no more input to build a token from, then we're done!
  if (!is)
    {
      _type=EOF_TOK;
      return;
    }

  int curr_state, // the state we are currently in while traversing the DFA
    prev_state;   // the previous state we were in while traversing the DFA

  char ch;  // current character read from input stream. 

  prev_state=curr_state=0; // initialy at start of DFA

  // as long as we are not in an ERROR state ...
  while(curr_state!=-1)
    {
      ch=is.get(); // get next character

      // about to move to next state.
      //   * "back up" current state to prev_state.
      //   * update current state. 
      prev_state=curr_state; 
      curr_state=DFA[curr_state][(int)ch];

      // if we didn't move into an error state, add this new character to lexeme
      if (curr_state!=-1)
	_value+=ch;
    }

  // we got a new token ...

  // set token's line number
  _line_num=__linenum;

  // prev_state will tell us where we were last OK, so use that to
  //   figure out token type based on DFA state number
  switch(prev_state)
    {
    case 1: // ID
      {
	_type=ID;

	// might actually be a keyworkd, so check against the array of keywords
	for (int tt=0; tt<END-OR; tt++)
	  {
	    if (reserved[tt] == _value)
	      {
		_type=(TokenType) (INTEGER+tt);
		break;
	      }
	  }
	break;
      }
    case 2: // NUM_INT
      {
	_type=NUM_INT;
	break;
      }
    case 4: // NUM_REAL
      {
	_type=NUM_REAL;
	break;
      }
    case 5: // ADDOP
      {
	_type=ADDOP;
	break;
      }
    case 6: // MULOP
      {
	_type=MULOP;
	break;
      }
    case 7: // TWO
    case 8: // RELOPS
      {
	_type=RELOP;
	break;
      }
    case 9: // ASSIGNOP
      {
	_type=ASSIGNOP;
	break;
      }
    case 10: //LPAREN
      {
	_type=LPAREN;
	break;
      }
    case 11: // RPAREN
      {
	_type=RPAREN;
	break;
      }
    case 13: //AND
      {
	_type=AND;
	break;
      }
    case 15: //OR
      {
	_type=OR;
	break;
      }
    case 16: // SEMICOLON
      {
	_type=SEMICOLON;
	break;
      }
    case 17: // LBRACK
      {
	_type=LBRACK; 
	break;
      }
    case 18: // RBRACK
      {
	_type=RBRACK;
	break;
      }
    case 19: // COMMA
      {
	_type=COMMA;
	break;
      }
    default: // anything other than th above is an ERROR
      {
	_type=ERROR;
	break;
      }
    }

  // if what we got was valid, we need to "unget" the extra character we
  //    read, as it might be part of the next Token.
  if (_type!=ERROR)
    is.unget();
}
