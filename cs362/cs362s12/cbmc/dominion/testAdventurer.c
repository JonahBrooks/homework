#include "dominion.h"

int nondet_int();

int main () {

  int res;

  /* Set up the test */
  int p = nondet_int();
  __CPROVER_assume((p >= 0) && (p < 4));
  int input1 = nondet_int();
  int input2 = nondet_int();
  int input3 = nondet_int();
  int advPos = nondet_int();

  struct gameState G0;
  struct gameState G1;
  __CPROVER_assume(G0 == G1);

  __CPROVER_assume(p == G0.whoseTurn);

  __CPROVER_assume(G0.handCount[p] > 0);
  __CPROVER_assume(G0.handCount[p] < MAX_HAND-2);

  __CPROVER_assume(G0.deckCount[p] >= 0);
  __CPROVER_assume(G0.deckCount[p] < MAX_DECK);
  __CPROVER_assume(G0.discardCount[p] >= 0);
  __CPROVER_assume(G0.discardCount[p] < MAX_DECK);
  __CPROVER_assume(G0.deckCount[p] + G0.discardCount[p] < MAX_DECK);

  __CPROVER_assume(advPos >= 0);
  __CPROVER_assume(advPos < G0.handCount[p]);
  __CPROVER_assume(G0.hand[p][advPos] == adventurer);

  int i;
  int c;
  int tc = 0;
  for (i = 0; i < G0.deckCount[p]; i++) {
    c = G0.deck[p][i];
    tc += ((c == copper) || (c == silver) || (c == gold));
  }
  for (i = 0; i < G0.discardCount[p]; i++) {
    c = G0.discard[p][i];
    tc += ((c == copper) || (c == silver) || (c == gold));
  }

  int tch = 0;
  for (i = 0; i < G0.handCount[p]; i++) {
    c = G0.hand[p][i];
    tch += ((c == copper) || (c == silver) || (c == gold));
  }

  __CPROVER_assume(tc >= 2);
  
  res = playCard(advPos, input1, input2, input3, &G1);
  
  int tc2 = 0;
  for (i = 0; i < G1.deckCount[p]; i++) {
    c = G1.deck[p][i];
    tc2 += ((c == copper) || (c == silver) || (c == gold));
  }
  for (i = 0; i < G1.discardCount[p]; i++) {
    c = G1.discard[p][i];
    tc2 += ((c == copper) || (c == silver) || (c == gold));
  }

  int tch2 = 0;
  for (i = 0; i < G1.handCount[p]; i++) {
    c = G1.hand[p][i];
    tch2 += ((c == copper) || (c == silver) || (c == gold));
  }
  
  assert(tch2 == tch + 2);
  assert(tc2 == tc - 2); 

}
