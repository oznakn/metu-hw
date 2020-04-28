CC=g++
CFLAGS=-c -ansi -Wall -pedantic-errors -O0

all: linkedlist browser

browser: main_browser.o tab.o webhistory.o
	$(CC) main_browser.o tab.o webhistory.o -o browser && rm -f *.o

linkedlist: main_linkedlist.o
	$(CC) main_linkedlist.o -o linkedlist && rm -f *.o

main_browser.o: main_browser.cpp
	$(CC) $(CFLAGS) main_browser.cpp -o main_browser.o

main_linkedlist.o: main_linkedlist.cpp
	$(CC) $(CFLAGS) main_linkedlist.cpp -o main_linkedlist.o

tab.o: Tab.cpp Tab.hpp
	$(CC) $(CFLAGS) Tab.cpp -o tab.o

webhistory.o: WebHistory.cpp WebHistory.hpp LinkedList.hpp Node.hpp Tab.hpp
	$(CC) $(CFLAGS) WebHistory.cpp -o webhistory.o

clean:
	rm *o
	rm linkedlist
	rm browser
