# This is (at least the start of) a makefile for CSC45500, 
#   project#1
## NOTE: any LINE starting with a '#' in a Makefile is a comment

#OPTIM=-O
DBG=-g
OPTS=$(DBG) $(OPTIM) -Wall -ansi -pedantic

CC=g++ $(OPTS)  -c
LNK=g++

OBJS= Parser.o Token.o Variable.o main.o
GLBL=Makefile Token.hpp

parser: $(OBJS)
	$(LNK) -o parser $(OBJS)

main.o: main.cpp Token.hpp $(GLBL)
	$(CC) main.cpp

Token.o: Token.cpp Token.hpp $(GLBL)
	$(CC) Token.cpp

Parser.o: Parser.cpp Parser.hpp Token.hpp Variable.hpp $(GLBL)
	$(CC) Parser.cpp


Variable.o: Variable.cpp Variable.hpp  $(GLBL)
	$(CC) Variable.cpp

clean:
	/bin/rm -f *.o parser *~
