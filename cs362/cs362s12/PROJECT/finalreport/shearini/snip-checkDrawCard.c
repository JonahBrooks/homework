void checkDrawCard(int p, struct gameState *post) {
	struct gameState pre;
	int r;


	if ((signal(SIGSEGV, segfault) == SIG_ERR))
	{
		printf("Could not set signal handler. %s\n", strerror(errno));
		return;
	}

	memcpy (&pre, post, sizeof(struct gameState));

#ifdef NOISY_TEST
	/* Where we started */
	printf ("  Before: p %d HC %d DeC %d DiC %d\n",
	        p, pre.handCount[p], pre.deckCount[p], pre.discardCount[p]);
#endif

	/* Run the function being tested */
	r = drawCard (p, post);

	if (r != 0)
	{
		printf("  drawCard returned non-zero value for p = %d\n", p);
		return;
	}

#ifdef NOISY_TEST
	/* Where we ended up */
	printf ("  After:  p %d HC %d DeC %d DiC %d\n",
	        p, post->handCount[p], post->deckCount[p], post->discardCount[p]);
#endif

	/* Do what the function should have done */
	if (pre.deckCount[p] > 0)
	{
		pre.handCount[p]++;
		pre.hand[p][pre.handCount[p]-1] = pre.deck[p][pre.deckCount[p]-1];
		pre.deckCount[p]--;
	}
	else if (pre.discardCount[p] > 0)
	{
		memcpy(pre.deck[p], post->deck[p], sizeof(int) * pre.discardCount[p]);
		memcpy(pre.discard[p], post->discard[p], sizeof(int)*pre.discardCount[p]);
		pre.hand[p][post->handCount[p]-1] = post->hand[p][post->handCount[p]-1];
		pre.handCount[p]++;
		pre.deckCount[p] = pre.discardCount[p]-1;
		pre.discardCount[p] = 0;
	}

	if (memcmp(&pre, post, sizeof(struct gameState)) != 0)
	{
		printf("  drawCard and reference differ\n");
		return;
	}

	printf("  ++pass++\n");
}
