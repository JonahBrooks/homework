#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define PLAYER_COUNT 2
#define SUPPLY_CARDS 16
#define DEBUG 0
#define NOISY_TEST 1

int checkDrawCard(int p, struct gameState *post) {
  struct gameState pre;
  memcpy (&pre, post, sizeof(struct gameState));

  int r;
  //  printf ("drawCard PRE: p %d HC %d DeC %d DiC %d\n",
  //	  p, pre.handCount[p], pre.deckCount[p], pre.discardCount[p]);
    
  r = drawCard (p, post);

  //printf ("drawCard POST: p %d HC %d DeC %d DiC %d\n",
  //      p, post->handCount[p], post->deckCount[p], post->discardCount[p]);

  if (pre.deckCount[p] > 0) {
    pre.handCount[p]++;
    pre.hand[p][pre.handCount[p]-1] = pre.deck[p][pre.deckCount[p]-1];
    pre.deckCount[p]--;
  } else if (pre.discardCount[p] > 0) {
    memcpy(pre.deck[p], post->deck[p], sizeof(int) * pre.discardCount[p]);
    memcpy(pre.discard[p], post->discard[p], sizeof(int)*pre.discardCount[p]);
    pre.hand[p][post->handCount[p]-1] = post->hand[p][post->handCount[p]-1];
    pre.handCount[p]++;
    pre.deckCount[p] = pre.discardCount[p]-1;
    pre.discardCount[p] = 0;
  }

  assert (r == 0);

  assert(memcmp(&pre, post, sizeof(struct gameState)) == 0);
  return 0;
}

int checkBuyCardSuccess(int p, struct gameState *post) 
{
  struct gameState pre;
  memcpy (&pre, post, sizeof(struct gameState));

  int r;
  r = buyCard (p, post);

  assert (r == 0);
  return 0;
}

int checkBuyCardFail(int p, struct gameState *post)
{
  struct gameState pre;
  memcpy (&pre, post, sizeof(struct gameState));

  int r;
    
  r = buyCard (p, post);

  assert (r != 0);
  return 0;
}

int checkCoins (struct gameState *post){
	struct gameState pre;
	memcpy (&pre, post, sizeof(struct gameState));

	int a, x;
	for(a = 0; a < post->handCount[whoseTurn(post)]; a++){
		if(post->hand[whoseTurn(post)][a] == copper ||
		   post->hand[whoseTurn(post)][a] == silver || 
		   post->hand[whoseTurn(post)][a] == gold){
			x++;
		}
	}
	return x;
}

	
int main () {
	
  int i, n, r, p, deckCount, discardCount, handCount, x, x2, y, y2;

  int k[10] = {adventurer, council_room, feast, gardens, mine,
               remodel, smithy, village, baron, great_hall};

  struct gameState H;
  struct gameState G;
  struct gameState F;

  initializeGame(PLAYER_COUNT, k, 11111, &G);
  initializeGame(PLAYER_COUNT, k, 100, &F);
  

  SelectStream(2);
  PutSeed(5);

  G.numBuys=300;
  printf("Testing whether all cards can be bought successfully...\n");

  for (n = 0; n < 17; n++) {
    G.coins=8;
    checkBuyCardSuccess(n, &G);
  }

  printf ("Test #1 passed\n");

  printf("Testing whether you can buy a card without the money to do so...\n");

  for (n = 1; n < 17; n++) {
    G.coins=-1;
    checkBuyCardFail(n, &G);
  }

  printf ("Test #2 passed\n");

  printf("Testing whether BuyCard decrements your coin count...\n");

  G.coins=5;
  checkBuyCardSuccess(2, &G);
  checkBuyCardFail(2, &G);

  printf ("Test #3 passed\n");

  printf("Testing whether BuyCard decrements your Buy count...\n");
  G.numBuys=2;
  G.coins=3000;
  checkBuyCardSuccess(2, &G);
  checkBuyCardSuccess(2, &G);
  checkBuyCardFail(2, &G);

  printf ("Test #4 passed\n");

  printf("Testing whether BuyCard decrements the supply count of the bought card...\n");
  G.supplyCount[duchy]=2;
  G.numBuys=3000;
  checkBuyCardSuccess(duchy, &G);
  checkBuyCardSuccess(duchy, &G);
  checkBuyCardFail(duchy, &G);

  printf ("Test #5 passed\n");

  printf("Testing whether the Adventurer card has the correct effect...\n");
  F.handCount[0] = 3;
  F.deckCount[0] = 30;
  F.deck[0][5] = copper;
  F.deck[0][10] = silver;
  F.hand[0][0] = silver;
  F.hand[0][1] = gold;
  F.hand[0][2] = adventurer;
  F.whoseTurn = 0;
  x=2;
  y=F.handCount[0];
  printf("x=%d\n",x);
  printf("y=%d\n",y);
  cardEffect(adventurer, 0, 0, 0, &F, 2, 0);
  x2=checkCoins(&F);
  printf("x2=%d\n",x2);
  y2=F.handCount[0];
  printf("y2=%d\n",y2);
  assert(x2==x+2);
  assert(y2==y+1);


  printf ("Test #6 passed\n");

  printf ("Testing drawCard with random tests.\n");

  SelectStream(2);
  PutSeed(3);

  for (n = 0; n < 2000; n++) {
    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&H)[i] = floor(Random() * 256);
    }
    p = floor(Random() * 2);
    H.deckCount[p] = floor(Random() * MAX_DECK);
    H.discardCount[p] = floor(Random() * MAX_DECK);
    H.handCount[p] = floor(Random() * MAX_HAND);
    checkDrawCard(p, &H);
  }

  printf ("ALL TESTS OK\n");

  exit(0);
  return 0;
}
