#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define DEBUG 0

int main () {

  struct gameState G;

  memset(&G, 23, sizeof(struct gameState)); 

  int k[10] = {adventurer, council_room, feast, gardens, mine,
	       remodel, smithy, village, baron, great_hall};

  printf ("Initializing game.\n");
  int r1 = initializeGame(2, k, 1, &G);
  assert (r1 == 0);
  printf ("Passed.\n");

  printf ("Drawing a card, where deck is not empty.\n");
  int p0DeckCount = G.deckCount[0];
  int p0HandCount = G.handCount[0];
  int p0TopCard = G.deck[0][p0DeckCount-1];
  int r2 = drawCard(0, &G);
  assert (r2 == 0);
  assert (G.deckCount[0] == (p0DeckCount - 1));
  assert (G.handCount[0] == (p0HandCount + 1));
  assert (G.hand[0][G.handCount[0]-1] == p0TopCard);
  printf ("Passed.\n");

  G.deckCount[0] = 0;
  printf ("Drawing a card, where deck and discard are empty.\n");
  p0DeckCount = G.deckCount[0];
  p0HandCount = G.handCount[0];
  p0TopCard = G.deck[0][p0DeckCount-1];
  int p0TopHandCard = G.hand[0][p0HandCount-1];
  r2 = drawCard(0, &G);
  assert (r2 == 0);
  printf ("Actual p0 deck count = %d\n", G.deckCount[0]);
  assert (G.deckCount[0] == 0);
  assert (G.handCount[0] == p0HandCount);
  assert (G.hand[0][G.handCount[0]-1] == p0TopHandCard);
  printf ("Passed.\n");

  int p = 0;

  printf ("GENERALIZED DRAWCARD TEST.\n");

  int pDeckCount = G.deckCount[p];
  int pTopCard;
  if (pDeckCount > 0)
    pTopCard = G.deck[p][pDeckCount-1];
  else
    pTopCard = -1;
  int pDiscardCount = G.discardCount[p];
  int pHandCount = G.handCount[p];

  int r = drawCard(p, &G);

  int pNewDeckCount = G.deckCount[p];
  int pNewHandCount = G.handCount[p];
  int pNewTopHandCard;
  if (pNewHandCount > 0)
    pNewTopHandCard = G.deck[p][pNewDeckCount-1];
  else
    pNewTopHandCard = -1;
  int pNewDiscardCount = G.discardCount[p];

  assert(r == 0);
  if (pDeckCount > 0) {
    assert (pNewHandCount == pHandCount + 1);
    assert (pNewTopHandCard == pTopCard);
    assert (pNewDiscardCount == pDiscardCount);
    assert (pNewDeckCount == pDeckCount - 1);
  }
  assert ((pNewHandCount == pHandCount + 1) || ((pDiscardCount == 0) && (pDeckCount == 0)));
  assert ((pNewTopHandCard == pTopCard) || (pDeckCount == 0));
  assert ((pNewDeckCount == pDeckCount - 1) || ((pDeckCount == 0)));
  assert 

  return 0;
}
