	/* Do what the function should have done */
	pre.phase = 1;
	if (memcmp(&pre, post, sizeof(struct gameState)) != 0)
	{
		/* If the states already match, the card was not bought, but buyCard should have returned -1. */
		printf("  buyCard didn't buy card for p = %d, c = %d\n", p, c);
		return;
	}
	else
	{
		pre.coins -= getCost(p);
		if (pre.coins < 0)
		{
			printf("  buyCard produced negative coinage for p = %d, c = %d\n", p, c);
			return;
		}

		/* Put card in discard */
		pre.discard[p][pre.discardCount[p]++] = c;
	}