all: monster world

.SILENT: run1 run2 run3 run4 all world monster clean

run1: monster world
	./world < inp1.txt

run2: monster world
	./world < inp2.txt

run3: monster world
	./world < inp3.txt

run4: monster world
	./world < inp4.txt

monster: monster.c
	gcc monster.c -g -o monster

world: world.c logging.c
	gcc world.c logging.c -g -o world

clean:
	rm -f world monster

pack:
	rm -f hw1.tar.gz
	tar czf hw1.tar.gz monster.c world.c Makefile

test:
	for number in 3 4 5 6 7 8 9 10; do \
        diff <(./world < "tests/inp$number.txt") "tests/out$number.txt"; \
    done
