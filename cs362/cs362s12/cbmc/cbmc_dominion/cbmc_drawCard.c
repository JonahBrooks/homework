#include "dominion.h"
#include "dominion_helpers.h"

int nondet_int();

int main() {
  struct gameState G;

  int p = nondet_int();
  __CPROVER_assume((p >= 0) && (p < 4));

  __CPROVER_assume(G.handCount[p] >= 0);
  __CPROVER_assume(G.handCount[p] < MAX_HAND);
  __CPROVER_assume(G.discardCount[p] >= 0);
  __CPROVER_assume(G.discardCount[p] < MAX_DECK);
  __CPROVER_assume(G.deckCount[p] >= 0);
  __CPROVER_assume(G.deckCount[p] < MAX_DECK);

  int r = drawCard(p, &G);
}
