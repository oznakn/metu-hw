run: compile
	./simulator

all: compile

compile: simulator.c defs.c defs.h helper.c helper.h writeOutput.c writeOutput.h
	$(CC) simulator.c defs.c helper.c writeOutput.c -lpthread -o simulator

valgrind_memcheck: compile
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./simulator

valgrind_deadlock: compile
	valgrind --tool=helgrind --read-var-info=yes ./simulator

gdb: compile
	gdb ./simulator

clean:
	rm -f simulator

pack:
	rm -f hw2.tar.gz
	tar czf hw2.tar.gz simulator.c defs.c defs.h Makefile
