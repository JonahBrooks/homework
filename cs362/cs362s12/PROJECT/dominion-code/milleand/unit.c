#include "unit.h"

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <string.h>

#include "dominion_helpers.h"
#include "dominion.h"
#include "rngs.h"
#include "util.h"

#define MAX_STR 80

struct gameState* g = NULL;

int randomInt(void);

void setUp();
void tearDown();

void TestCompare(GinkoTest* tc)
{
	const int nTests = 1000;
	int a;
	int b;
	int i;
	
	SelectStream(1);
	PutSeed(time(NULL));

	for (i = 0; i < nTests; i++)
	{
		a = randomInt();
		b = randomInt();

		if (a < b)
		{
			ginkoAssert(tc, "a < b did not return -1", compare(&a, &b) == -1);
		}
		else if (a > b)
		{
			ginkoAssert(tc, "a > b did not return 1", compare(&a, &b) == 1);
		}
		else
		{
			ginkoAssert(tc, "a == b did not return 0", compare(&a, &b) == 0);
		}
	}
}

void TestInitializeGame(GinkoTest* tc)
{
	struct gameState G;
	char str[MAX_STR];

    int i;
    int start = -1;
    int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
        sea_hag, tribute, smithy};

    memset(&G, 'z', sizeof(struct gameState));

    initializeGame(4, k, time(NULL), &G);

    /*printf("Rough guide to locations in structure:\n");
    printf("0: numPlayers\n");
    printf("%ld: supplyCount[0]\n", ((long)&(G.supplyCount[0]))-((long)&G));
    printf("%ld: embargoTokens[0]\n", ((long)&(G.embargoTokens[0]))-((long)&G));
    printf("%ld: hand[0][0]\n", ((long)&(G.hand[0][0]))-(long)(&G));
    printf("%ld: deck[0][0]\n", ((long)&(G.deck[0][0]))-(long)(&G));
    printf("%ld: discard[0][0]\n", ((long)&(G.discard[0][0]))-(long)(&G));
    printf("%ld: playerCards[0]\n", ((long)&(G.playedCards[0]))-(long)(&G));*/

    for (i = 0; i < sizeof(struct gameState); i++)
	{
        if (((char*)&G)[i] == 'z')
		{
            if (start == -1)
			{
                start = i;
            }
        }
		else
		{
            if (start != -1)
			{
                if (start == (i - 1))
				{
					snprintf(str, MAX_STR - 1, "Byte %d uninitialized.\n", start);
					ginkoFail(tc, str);
                }
				else
				{
					snprintf(str, MAX_STR - 1, "Bytes %d-%d uninitialized.\n", start, i - 1);
					ginkoFail(tc, str);
                }

                start = -1;
            }
        }
    }

    if (start != -1)
	{
        if (start == (i - 1))
		{
			snprintf(str, MAX_STR - 1, "Byte %d uninitialized.\n", start);
			ginkoFail(tc, str);
        }
		else
		{
			snprintf(str, MAX_STR - 1, "Bytes %d-%d uninitialized.\n", start, i - 1);
			ginkoFail(tc, str);
        }
    }
}

void TestShuffle(GinkoTest* tc)
{
	struct gameState G2;
	int ret;

	// Initialize G.
    int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
        sea_hag, tribute, smithy};

	setUp();

	memcpy(&G2, g, sizeof(struct gameState));
	ret = shuffle(0, g);

	if (g->deckCount[0] > 0)
	{
		ginkoAssert(tc, "Shuffle failed", ret != -1);
    
		qsort((void*)(g->deck[0]), g->deckCount[0], sizeof(int), compare);
		qsort((void*)(G2.deck[0]), G2.deckCount[0], sizeof(int), compare);    
	}
	else
	{
		ginkoAssert(tc, "Shuffle of an empty deck did not fail", ret == -1);
	}

	ginkoAssert(tc, "Game states do not match", memcmp(g, &G2, sizeof(struct gameState)) == 0);

	tearDown();
}

void TestPlayCard(GinkoTest* tc)
{
	setUp();
	ginkoFail(tc, "Unimplemented");
	tearDown();
}

void TestBuyCard(GinkoTest* tc)
{
	setUp();
	ginkoFail(tc, "Unimplemented");
	tearDown();
}

void TestNumHandCards(GinkoTest* tc)
{
	setUp();
	ginkoFail(tc, "Unimplemented");
	tearDown();
}

void TestHandCard(GinkoTest* tc)
{
	const int nTests = 1000;
	int handPos;
	int turn;
	int i;
	
	setUp();
	
	for (i = 0; i < nTests; i++)
	{
		turn = randomIntRange(-1, MAX_PLAYERS);
		handPos = randomIntRange(-1, MAX_HAND);
		g->whoseTurn = turn;
		
		if ((turn < 0) || (turn >= g->numPlayers))
		{
			ginkoAssert(tc, "Did not fail for invalid turn", handCard(handPos, g) == -1);
		}
		else
		{
			if ((handPos < 0) || (handPos >= g->handCount[turn]))
			{
				ginkoAssert(tc, "Did not fail for invalid hand position", handCard(handPos, g) == -1);
			}
			else
			{
				ginkoAssert(tc, "Did not return correct card", handCard(handPos, g) == g->hand[turn][handPos]);
			}
		}
	}

	tearDown();
}

void TestSupplyCount(GinkoTest* tc)
{
	setUp();
	ginkoFail(tc, "Unimplemented");
	tearDown();
}

void TestFullDeckCount(GinkoTest* tc)
{
	setUp();
	ginkoFail(tc, "Unimplemented");
	tearDown();
}

void TestWhoseTurn(GinkoTest* tc)
{
	const int nTests = 1000;
	int turn;
	int i;

	setUp();
	
	for (i = 0; i < nTests; i++)
	{
		// Generate an integer beween -1 and g->numPlayers + 1
		turn = randomIntRange(-1, MAX_PLAYERS);
		g->whoseTurn = turn;

		if ((turn < 0) || (turn >= g->numPlayers))
		{
			ginkoAssert(tc, "Did not fail for invalid turn", whoseTurn(g) == -1);
		}
		else
		{
			ginkoAssert(tc, "Did not return correct turn", whoseTurn(g) == turn);
		}
	}

	tearDown();
}

void TestEndTurn(GinkoTest* tc)
{
	setUp();
	ginkoFail(tc, "Unimplemented");
	tearDown();
}

void TestIsGameOver(GinkoTest* tc)
{
	setUp();
	ginkoFail(tc, "Unimplemented");
	tearDown();
}

void TestScoreFor(GinkoTest* tc)
{
	setUp();
	ginkoFail(tc, "Unimplemented");
	tearDown();
}

void TestGetWinners(GinkoTest* tc)
{
	setUp();
	ginkoFail(tc, "Unimplemented");
	tearDown();
}

int randomInt(void)
{
	return (Random() > 0.5f) ? Random() * INT_MAX : Random() * INT_MIN;
}

int randomIntRange(int min, int max)
{
	assert(min < max);
	return floor(Random() * (max + 1 - min)) + min;
}

void setUp()
{
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
        sea_hag, tribute, smithy};

	if (g)
	{
		free(g);
		g = NULL;
	}

	g = (struct gameState*)malloc(sizeof(struct gameState));
	assert(g);

    memset(g, 'z', sizeof(struct gameState));
    initializeGame(4, k, time(NULL), g);
}

void tearDown()
{
	if (g)
	{
		free(g);
		g = NULL;
	}
}
