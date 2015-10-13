#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define DEBUG 0
#define NOISY_TEST 1

int checkBuyCardPass(int p, struct gameState *post) {
	struct gameState pre;
	memcpy (&pre, post, sizeof(struct gameState));

	int r;
	printf ("buyCard PRE: p %d HC %d DeC %d DiC %d\n", p, pre.handCount[p], pre.deckCount[p], pre.discardCount[p]);
	r = buyCard (p, post);
	printf ("buyCard POST: p %d HC %d DeC %d DiC %d\n", p, post->handCount[p], post->deckCount[p], post->discardCount[p]);
	
	assert (r == 0);
	return 0;
}

int checkBuyCardFail(int p, struct gameState *post){
	struct gameState pre;
	memcpy (&pre, post, sizeof(struct gameState));
	
	int r;
	printf ("buyCard PRE: p %d HC %d DeC %d DiC %d\n", p, pre.handCount[p], pre.deckCount[p], pre.discardCount[p]);
	r = buyCard (p, post);
	printf ("buyCard POST: p %d HC %d DeC %d DiC %d\n", p, post->handCount[p], post->deckCount[p], post->discardCount[p]);
	
	assert (r != 0);
	return 0;
}

int checkCoins (struct gameState *post){
	int a, x;
	for(a = 0; a < post->handCount[post->whoseTurn]; a++){
		if(post->hand[post->whoseTurn][a] == copper || post->hand[post->whoseTurn][a] == silver || post->hand[post->whoseTurn][a] == gold){
			x++;
		}
		a++;
	}
	return x;
}
 
 int main () {
	int i, p;
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
	
	struct gameState G;
	initializeGame(4, k, 11111, &G);
	
	printf ("RANDOM TESTS.\n");
	SelectStream(2);
	PutSeed(3);
	G.numBuys = 30000;
	
	//Making sure buying a card works
	for(p = 0; p < 17; p++){
		G.coins = 8;
		checkBuyCardPass(p, &G);
	}
	printf ("Buying any cards test passed\n");
	
	//Making sure buying a card with no coins fails
	for(p = 0; p < 17; p++){
		G.coins = -1;
		checkBuyCardFail(p, &G);
	}
	printf ("Buy fail test passed\n");

	//Testing for buying two cards
	G.coins = 5;
	checkBuyCardPass(silver, &G);
	checkBuyCardFail(silver, &G);
	printf ("Double card buy test passed\n");
	
	//Test to make sure there is a card there to buy
	G.coins = 8;
	G.supplyCount[silver] = 1;
	checkBuyCardPass(silver, &G);
	checkBuyCardFail(silver, &G);
	
	//Testing if you have a buy action left
	G.coins = 70;
	G.numBuys = 1;
	checkBuyCardPass(gold, &G);
	checkBuyCardFail(gold, &G);
	
	//Checks to see that two treasure cards were added
	p = floor(Random() * 2);
	G.deckCount[p] = floor(Random() * MAX_DECK);
	G.discardCount[p] = floor(Random() * MAX_DECK);
	G.handCount[p] = floor(Random() * MAX_HAND);
	G.handCount[2] = adventurer;
	i = checkCoins(&G);
	cardEffect(adventurer, 0, 0, 0, &G, 2, 0);
	assert(checkCoins(&G) == i + 2);
	
	printf ("ALL TESTS OK\n");
	printf ("SIMPLE FIXED TESTS.\n");
	
	exit (0);
	return 0;
}
