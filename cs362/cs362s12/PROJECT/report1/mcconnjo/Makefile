CFLAGS = -fprofile-arcs -ftest-coverage -Wall

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g -Wall $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g -Wall $(CFLAGS)

testDom: testBuyCard.c dominion.o rngs.o
	gcc -o testDom -g -lm testBuyCard.c dominion.o rngs.o $(CFLAGS)

clean:
	rm -f *.o playdom.exe playdom test.exe test player player.exe testInit testInit.exe *.gcov *.gcda *.gcno
