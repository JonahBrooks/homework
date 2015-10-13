int main () {
	int n, i, p;
	struct gameState G;


	printf ("Testing drawCard randomly\n");

	SelectStream(2);
	PutSeed(3);

	for (n = 0; n < NUM_TESTS; n++) {
		for (i = 0; i < sizeof(struct gameState); i++) {
			((char*)&G)[i] = (int)floor(Random() * 256);
		}
		p = (int)floor(Random() * 2);
		G.deckCount[p] = (int)floor(Random() * MAX_DECK);
		G.discardCount[p] = (int)floor(Random() * MAX_DECK);
		G.handCount[p] = (int)floor(Random() * MAX_HAND);
		checkDrawCard(p, &G);
	}

	printf ("End of drawCard tests\n\n");

	return 0;
}
