#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "rngs.h"
#include "dominion.h"
#include "dominion_helpers.h"

static int seeds = 1;
int randnum ();
struct	gameState* makedefault (int players);
void print_state (struct gameState *game);
void print_amount (struct gameState *game);
int checkBuyCard ();
int otherTests ();

int main (int argc, char *argv[]) {
	checkBuyCard ();
	return 0;
}

struct gameState* makedefault(int players){
	struct gameState * game = (struct gameState *)malloc(sizeof(struct gameState));
	int k[10] = {adventurer, council_room, feast, gardens, mine,
	 					remodel, smithy, village, baron, great_hall};
	if(initializeGame(players, k, 2, game)==-1){
		printf("Failed to initialize game. \n");
	}
	return game;
}

int checkBuyCard () {

	/* random testing */

	int player;
	int i = 1;
	int buyval;
	int val = randnum ();
	int turn = 0;
	int numplayers = val%3 + 2;
	struct gameState* game = makedefault(2);
	game->coins= 20;
	game->numBuys = val%3;

	int test_turn = 0;
	while( test_turn < 100){
		test_turn ++;
		val = randnum();
		player = whoseTurn(game);
		
		switch(val%7){
		
		case 1:
			turn++;
			print_state(game);
			printf("Player %d has a score of %d: turn %d\n\n", whoseTurn(game), scoreFor(whoseTurn(game),game),turn/2);
			
			if(endTurn(game)== -1){
				i = 0;
			}
			else{
				game->coins= 20;
				game->numBuys = val%3;
			}
			break;
		case 2:
		default:
			buyval = val%27;
			int coin = game->coins;;
			printf("Buying card: %d\t",buyval);
			if(buyCard(buyval, game)!= -1){
				printf("Bought card: %d\n",buyval);
				game->coins = coin - getCost(buyval);
				if(game->phase == 0){
					printf("Buying doesnt change phase.\n");
					i = 0;
				}
			}
			else{
				printf("\n");
			}
			break;
		}
	}
	
	
	
	/* other tests */
	
	otherTests ();


	return 0;
}

int randnum () {
	srand( time(NULL) );
	int rnum = rand ();
	return rnum;
}

int otherTests () {
	printf("\n\n--------------------test buy---------------------\n");
	struct gameState* game2 = makedefault(2);
	game2->coins = 10;
	game2->numBuys = 1;
	int supcount = game2->supplyCount[3];
	int disccount = game2->discardCount[0];
	if(buyCard(3,game2)!=-1){
		if(game2->coins!= 10 -getCost(3)){
			printf("Doesn't update coins.\n");
		}
		if(game2->numBuys != 0){
			printf("Doesn't update buys.\n");
		}
		if(game2->supplyCount[3]!= supcount -1){
			printf("Doesn't update supply count.\n");
		}
		if(game2->discardCount[0]!= disccount+1){
			printf("Doesn't update discard count.\n");
		}
		else{
			if(game2->discard[0][disccount]!=3){
				printf("Doesn't add card to top of discard pile.\n");
			}
		}
	}

	game2->coins =0;
	game2->numBuys = 1;
	if(buyCard(3,game2)!=-1){
		printf("Doesn't check for coins.\n");
	}

	game2->coins =20;
	game2->numBuys = 0;
	if(buyCard(3,game2)!=-1){
		printf("Doesn't check for buys.\n");
	}
	return 0;
}

void print_state(struct gameState *game) {
	int numActions = game->numActions;
	int numCoins = game->coins;
	int numBuys = game->numBuys;
	int currentPlayer = game->whoseTurn;
	
	printf("Player %d:\t%d actions\t%d coins\t%d buys\n", currentPlayer, numActions, numCoins, numBuys);
	print_amount(game);
}

void print_amount(struct gameState *game) {
	int currentPlayer = game->whoseTurn;
	printf("Hand: %d\tdeck = %d\t Discard: %d \n", game->handCount[currentPlayer], game->deckCount[currentPlayer], game->discardCount[currentPlayer]);
}
