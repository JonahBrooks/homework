#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rngs.h"
#include <math.h>

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef NOISY_TEST
#define NOISY_TEST 1
#endif

#define NUM_RDM 100    /* Number of random tests to run */
#define MAX_COIN 10    /* For random coin amount        */

void checkBuyCard(struct gameState *state, int fromSupply);

void checkBuyCard(struct gameState *state, int fromSupply) {
    struct gameState pre;
    memcpy (&pre, state, sizeof(struct gameState));
    
    buyCard(fromSupply, state);
    
    /* There's enough cards in the supply and the player has enough money */
    if(     pre.supplyCount[fromSupply] > 0 
         && pre.coins >= getCost(fromSupply)
         && pre.numBuys > 0 ) {
        
        pre.coins -= getCost(fromSupply);
        pre.supplyCount[fromSupply]--;
        pre.numBuys--;
        pre.discard[pre.whoseTurn][pre.discardCount[pre.whoseTurn]] = fromSupply;
        pre.discardCount[pre.whoseTurn]++;
    }

    assert(memcmp(&pre, state, sizeof(struct gameState)) == 0);
}

int main () {
	struct gameState G;
	printf("Testing buyCard\n\n");

	printf("%d Simple Random Tests\n\n", NUM_RDM);
	
	SelectStream(2);
	PutSeed(3);
	
    int supplyPos;
	int player;  
	int i;
	int j;
	for(i = 0; i < NUM_RDM; ++i) {
        for(j = 0; j < sizeof(struct gameState); j++) {
            ((char*)&G)[j] = floor(Random() * 256);
        }
		/* Buying phase should be 1 */
		G.phase = 1;
		
		/* RANDOM VALUES */
		player = floor(Random() * 2);
        G.whoseTurn = player;
		G.coins = floor(Random() * MAX_COIN);
		G.numBuys = floor(Random() * 10);
		G.discardCount[player] = floor(Random() * MAX_DECK);
		supplyPos = floor(Random() * (treasure_map + 1));
        G.supplyCount[supplyPos] = floor(Random() * 5);
        
        /* EXECUTE BUYCARD */
        checkBuyCard(&G, supplyPos);
	}
    
    
	return 0;
}

