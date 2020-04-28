CC=g++
CFLAGS=-c -ansi -Wall -pedantic-errors -O0

all: bst invertedindex

invertedindex: main_invertedindex.o iidata.o invertedindex.o
	$(CC) main_invertedindex.o iidata.o invertedindex.o -o invertedindex && rm -f *.o

bst: main_bst.o
	$(CC) main_bst.o -o bst && rm -f *.o

main_invertedindex.o: main_invertedindex.cpp
	$(CC) $(CFLAGS) main_invertedindex.cpp -o main_invertedindex.o

main_bst.o: main_bst.cpp
	$(CC) $(CFLAGS) main_bst.cpp -o main_bst.o

iidata.o: IIData.cpp IIData.hpp
	$(CC) $(CFLAGS) IIData.cpp -o iidata.o

invertedindex.o: InvertedIndex.cpp InvertedIndex.hpp BST.hpp BSTNode.hpp IIData.hpp
	$(CC) $(CFLAGS) InvertedIndex.cpp -o invertedindex.o

clean:
	rm *o
	rm bst
	rm invertedindex
