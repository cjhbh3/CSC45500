/*******************************************
 * File: main.cpp                          *
 * Author: CJ Hess                       *
 * Date: 3/23/2023                           *
 * PURPOSE: driver file for project 3      *
 *******************************************/

#include <iostream>
#include <fstream>
#include <string>

#include "Parser.hpp"

using namespace std;

struct node {
  int startAdr;
  int endAdr;
  int refCount;
  node * next = NULL;
};

struct variable {
  int startAdr;
  int endAdr;
  int refCount;
  int name;
};

/*
  Looks like we will need structs/classes to implement the freelist and hold on to variables
*/

int main(int argc, char *argv[])
{
  // prompt for initial block size for the freeList
  int freeListSize = 0;
  string inputFileName;
  cout << "Please enter the initial freelist (heap) size: ";
  cin >> freeListSize;

  cout << "Please enter the name of an input file: ";
  cin >> inputFileName;

  Parser p(inputFileName.c_str());

  if (p.prog()) {
    cout << "Success" << endl;
  }

  return 0;
}





