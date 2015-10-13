CFLAGS = -fprofile-arcs -ftest-coverage -Wall

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g -Wall $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g -Wall $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g -lm dominion.o rngs.o $(CFLAGS)

testbuy: dominion.o testSuite.c
	gcc -o testBuyCard testBuyCard.c -g -lm dominion.o rngs.o $(CFLAGS)

testdom: dominion.o testSuite.c
	gcc -o testdom testSuite.c -g -lm dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g -Wall $(CFLAGS)

player: player.c interface.o
	gcc -o player player.c -g -lm dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player

clean:
	rm -f *.o playdom.exe test.out playdom test.exe test testdom player player.exe testInit testInit.exe *.gcov *.gcda *.gcno
