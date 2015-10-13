#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int checkBuyCard(struct gameState *state) {
	

  int i, p, n, r, c, card, cost, before, after, discarded, numBuys, who;

  int k[10] = {adventurer, council_room, feast, gardens, mine,
	       remodel, smithy, village, baron, great_hall};

  SelectStream(2);
  PutSeed(3);
  p = Random() * 2;
  n = Random() * 100;
  card = Random() * 27;
  initializeGame(p, k, n, state);
  who = whoseTurn(state);
  for(i=0; i<5;i++){
	state->hand[who][i] = copper;
  }
  discarded = state->discardCount[who];
  cost = getCost(card);
  before = updateCoins(who, state, 0);
  state->numBuys = 1;
  numBuys = state->numBuys;
  
  printf("player %d has %d buys and %d coins and wishes to buy for %d coins.\n", who, numBuys, before, cost);

  r = buyCard (c, state);
  
  after = updateCoins(who, state, 0);
  assert (r == 0);
  assert (cost <= before - after);
  assert (state->discard[who][state->discardCount[who]] == card);
  assert (state->numBuys == numBuys-1);
  
  return 0;
}

int main () {

  int n;
  struct gameState G;

  printf ("RANDOM TESTS.\n");

  for (n = 0; n < 10; n++) {
    checkBuyCard(&G);
  }

  printf ("ALL TESTS OK\n");

  return 0;
}
