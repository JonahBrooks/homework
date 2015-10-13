CFLAGS = -fprofile-arcs -ftest-coverage -Wall

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g -Wall $(CFLAGS)

dominion.o: dominion.h dominion.c dominion_helpers.h rngs.o
	g++ -c dominion.c -g -Wall $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g -lm dominion.o rngs.o $(CFLAGS)

test: dominion.o rngs.o test.cpp dominion_helpers.h
	g++ -o test test.cpp -g -lm dominion.o rngs.o $(CFLAGS)
	
testdom: dominion.o rngs.o test.cpp dominion_helpers.h
	g++ -o test test.cpp -g -lm dominion.o rngs.o $(CFLAGS)
	
interface.o: interface.h interface.c
	gcc -c interface.c -g -Wall $(CFLAGS)

#player: player.c interface.o
#	gcc -o player player.c -g -lm dominion.o rngs.o interface.o $(CFLAGS)

#all: playdom player
all: playdom test

clean:
	rm -f *.o playdom.exe playdom test.exe test player player.exe testInit testInit.exe *.gcov *.gcda *.gcno
