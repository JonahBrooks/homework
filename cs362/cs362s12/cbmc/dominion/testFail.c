#include "dominion.h"

int nondet_int();

int main () {

  int i;
  int res;

  /* Set up the test */
  int input1 = nondet_int();
  int input2 = nondet_int();
  int input3 = nondet_int();

  struct gameState G0;
  struct gameState G1;
  __CPROVER_assume(G0 == G1);

  int p = nondet_int();
  __CPROVER_assume((p >= 0) && (p < MAX_PLAYERS));

  __CPROVER_assume(p == G0.whoseTurn);

  __CPROVER_assume(G0.handCount[p] >= 0);
  __CPROVER_assume(G0.handCount[p] < MAX_HAND);
  
  __CPROVER_assume(G0.deckCount[p] >= 0);
  __CPROVER_assume(G0.deckCount[p] < MAX_DECK);
  __CPROVER_assume(G0.discardCount[p] >= 0);
  __CPROVER_assume(G0.discardCount[p] < MAX_DECK);
  __CPROVER_assume(G0.deckCount[p] + G0.discardCount[p] < MAX_DECK);

  int hp = nondet_int();
  __CPROVER_assume((hp >= 0) && (hp < G0.handCount[p]));

  int shouldFailPlayCard = 0;
  shouldFailPlayCard = shouldFailPlayCard || (G0.numActions <= 0);
  shouldFailPlayCard = shouldFailPlayCard || (G0.handCount[p] < 1); 
  shouldFailPlayCard = shouldFailPlayCard || (G0.phase != 0);
  shouldFailPlayCard = shouldFailPlayCard || (G0.hand[p][hp] < adventurer);
  shouldFailPlayCard = shouldFailPlayCard || (G0.hand[p][hp] > treasure_map);
  //__CPROVER_assume(shouldFail);

  int sp = nondet_int();
  __CPROVER_assume(sp >= 0);
  __CPROVER_assume(sp <= treasure_map); 

  int shouldFailBuyCard = 0;
  shouldFailBuyCard = shouldFailBuyCard || (G0.numBuys <= 0);
  shouldFailBuyCard = shouldFailBuyCard || (G0.supplyCount[sp] <= 0); 

  int play = nondet_int();
  if (play) {
    __CPROVER_assume(shouldFailPlayCard);
    res = playCard(hp, input1, input2, input3, &G1);
  } else {
    __CPROVER_assume(shouldFailBuyCard);
    res = buyCard(sp, &G1);
  }

  assert (res == -1);
  assert (G0 == G1);

}
