
CXX = g++
CXX_FLAGS = -std=c++14 -g -ansi -Wall -O2

FILES = HashUtils.o main.cpp

all:
	$(CXX) $(CXX_FLAGS) -c HashUtils.cpp -o HashUtils.o

	$(CXX) $(CXX_FLAGS) $(FILES) -o Test

	rm -f *.o


clean:
	rm -f Test
	rm -f *.o
