int main (int argc, char **argv)
{
	int i, n, r, seed;
	int k[10] = {adventurer, council_room, feast, gardens, mine,
	             remodel, smithy, village, baron, great_hall};


	struct gameState G;
	int op;
	int card;
	int val;


	printf ("Testing Dominion randomly\n");

	SelectStream(2);
	PutSeed(3);

	for (seed = 1; seed < NUM_GAMES; seed++)
	{
		initializeGame(2, k, seed, &G);

		for (n = 0; n < GAME_LENGTH; n++)
		{
#ifdef NOISY_TEST
			printf ("  <%d,%d> State: turn: %d, in hand: ", seed, n,
			        G.whoseTurn);
#endif
			for (i = 0; i < G.handCount[G.whoseTurn]; i++)
			{
				card = G.hand[G.whoseTurn][i];
#ifdef NOISY_TEST
				printf ("%d ", card);
#endif
				if ((card < 0) || (card > treasure_map))
				{
					printf ("\n  Stop: Invalid card in hand: %d\n", card);
					return 0;
				}
			}

			op = (int)floor(Random() * 3);
			card = (int)(floor(Random() * (treasure_map+2))) - 1;
			val = (int)floor(Random() * 5);
			switch (op)
			{
				case 0:
#ifdef NOISY_TEST
					printf ("> ENDTURN: ");
#endif
					r = endTurn(&G);
					if (r != 0)
					{
						printf("  endTurn returned non-zero value: %d\n", r);
					}
					break;
				case 1:
#ifdef NOISY_TEST
					printf ("> BUY %d: ", card);
#endif
					r = buyCard(card, &G);
					if (r != 0)
					{
						printf("  buyCard returned non-zero value: %d for card: %d\n", r, card);
					}
					break;
				case 2:
#ifdef NOISY_TEST
					printf ("> PLAY %d: ", val);
#endif
					r = playCard(val, -1, -1, -1, &G);
					if (r != 0)
					{
						printf("  playCard returned non-zero value: %d for val: \n", r, val);
					}
					break;
			}
#ifdef NOISY_TEST
			printf ("\n");
#endif
		}

		printf ("  Game over\n");
	}

	printf ("End of Dominion tests\n");

	return 0;
}
