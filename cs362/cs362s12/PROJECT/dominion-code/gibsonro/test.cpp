#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include "dominion_helpers.h"
#include "dominion.h"
#include "rngs.h"
using namespace std;


#define ACTION_COUNT 250
#define DEBUG 0

string result = "";

int gameStateComparer (gameState *a, gameState *b);
void printCurrentStats(gameState *state);



int main(int argc, char** argv) {

    struct gameState *preState = (struct gameState *) malloc(sizeof(struct gameState));
    struct gameState *postState = (struct gameState *) malloc(sizeof(struct gameState));

    memset(preState, 0, sizeof(struct gameState));
    int shift =(int) floor(Random() * 12);
    int cards[10] = { 7 + shift, 8 + shift, 9 + shift, 10 + shift, 11 + shift, 12 + shift, 13 + shift, 14 + shift, 15 + shift, 16 + shift };
    string repeatCount;
    long seed;
    long repeats;
    int targetCard;
    int i, k;

    int actions[ACTION_COUNT];
    

    
    
	if (argc !=3) {
		printf("Syntax: testdom <run-count> <seed>");
		return -1;
	}
	else {
        seed = atol(argv[2]);
		repeats = atol(argv[1]);
		SelectStream(1);
		PutSeed(seed);
	}

    for (i = 0; i < ACTION_COUNT; ++i) {
        actions[i] = (int) floor(Random() * 3);
    }




        int j, playResult, targetCardPosition, actionCount, buyCount, handCount, attempts, usingStrategy;

        for (i = 0; i < repeats; ++i) {
            
            initializeGame((int)floor(Random() * (MAX_PLAYERS)) + 2, cards, floor(Random() * seed * 5), preState);

            usingStrategy = (int)floor(Random() * 2);

            memcpy(postState, preState, sizeof(struct gameState));
            for (j = 0; j < ACTION_COUNT; ++j) {

                

                switch (actions[j]) {
                case 0:
                    playResult = -1;
                    targetCard = (int) floor(Random() * (treasure_map + 10)) - 20;
                    attempts = 10;

                    while (playResult == -1 && postState->numBuys > 0 && attempts > 0) {
                        


                        buyCount = postState->numBuys;
                        targetCard = (int) floor(Random() * treasure_map + 1) - 1;
                        printCurrentStats(postState);
                        
                        // one half of the time, use a real strategy to buy
                        if (usingStrategy) {

                            if (postState->coins >= 8) {
                                targetCard = 3;
                            }
                            else if (postState->coins >= 6){
                                if (j % 2 == 0 && fullDeckCount(postState->whoseTurn, adventurer, postState) < 3 && postState->supplyCount[adventurer] > 0) {
                                    targetCard = adventurer;
                                }
                                else {
                                    targetCard = gold;
                                }
                            }
                            else if (postState->coins >= 3) {
                                targetCard = silver;
                            }
                        }
                        
                        cout << "Buying: " << targetCard << "(" << getCost(targetCard) << "coins)" << endl;
                        

                        
                        playResult = buyCard(targetCard, postState);
                        cout << "buyCard returned: " << playResult << endl;
                        printCurrentStats(postState);

                        if (playResult >= 0) {
                            assert(postState->numBuys < buyCount) ;
                        }
                        --attempts;
                    }
                    if (attempts == 0) {
                        cout << "Failed to buy any cards after 10 attempts." << endl;
                    }
                    break;

                case 1:
                    targetCardPosition = (int) floor(Random() * numHandCards(postState));

                    actionCount = postState->numActions;
                    handCount = postState->handCount[postState->whoseTurn];

                    if (usingStrategy) {
                        for (k = 0; k < postState->handCount[postState->whoseTurn]; ++k) {
                            if (postState->hand[postState->whoseTurn][i] == adventurer) {
                                targetCardPosition = k;
                                break;
                            }
                        }
                    }

                    targetCard = postState->hand[postState->whoseTurn][targetCardPosition];
                    
                    printCurrentStats(postState);
                    playResult = playCard(targetCardPosition, (int) floor(Random() * postState->handCount[postState->whoseTurn]), (int) floor(Random() * postState->handCount[postState->whoseTurn]), (int) floor(Random() * postState->handCount[postState->whoseTurn]), postState);
                    cout << "Playing a " << targetCard << " from position " << targetCardPosition << endl;
                    cout << "playCard returned: " << playResult << endl;
                    printCurrentStats(postState);
                    if (playResult > -1 && postState->handCount[postState->whoseTurn] < handCount) {
                        assert(postState->numActions < actionCount);
                    }
                    printCurrentStats(postState);
                    break;

                case 2:
                    cout << "Ending player " << whoseTurn(postState) << "'s turn" << endl;
                    endTurn(postState);
                    break;

                default:
                    break;

                }
                if (isGameOver(postState)) 
                {
                    cout << "Finished a game! Holy cow!" << endl;
                    int winners[4];
                    getWinners(winners, postState);
                    for (k = 0; k < 4; ++k) {
                        if (winners[k]) {
                            cout << "Winner! Player " << k + 1 << endl;
                        }
                    }
                    break;
                }
            }


        memcpy(postState, preState, sizeof(struct gameState));

        /* 
         * first test: buying a card twice with only 1 buy should keep the same
         * cards in the discard as buying it once
         */
        
        targetCard = (int) floor(Random() * 17);
        
        buyCard(targetCard, postState);
        cout << "gameState comparison #1: after buyCard" << endl;
        
        
        if (postState->numBuys == 0)  {
            assert(gameStateComparer(preState, postState) != 0);
            assert(postState->playedCardCount > 0);
            assert(memcmp(preState->playedCards, postState->playedCards, sizeof(int) * 500) != 0);
        }
        

        
        
        memcpy(preState->discard, postState->discard, sizeof(int) * 2000);

        if (postState->numBuys == 0) {
            assert(memcmp(preState->hand[0], postState->hand[0], sizeof(int) * MAX_HAND) != 0);
        }
        
        preState->handCount[0] = postState->handCount[0];
        preState->supplyCount[targetCard] = postState->supplyCount[targetCard];
        preState->phase = postState->phase;
        preState->playedCardCount = postState->playedCardCount;
        preState->coins = postState->coins;
        preState->numBuys = postState->numBuys;
        memcpy(preState->hand[0], postState->hand[0], sizeof(int) * MAX_HAND);

        buyCard(targetCard, postState);
        cout << "gameState comparison #2: after buyCard twice" << endl;
        
        if (gameStateComparer(preState, postState) != 0)
            assert(memcmp(preState, postState, sizeof(struct gameState)) == 0);
        


        /* 
         * Second test: is buyCard really taking cards?
         * Try buying a copper, should be fewer coppers
         */

        
        targetCard = copper;

        initializeGame(floor(Random() * (MAX_PLAYERS - 2)) + 2, cards, seed, preState);
        memcpy(postState, preState, sizeof(struct gameState));
        
        buyCard(targetCard, postState);
        cout << "gameState comparison #3: after buyCard for copper" << endl;
        assert(gameStateComparer(preState, postState) != 0); 

        assert(postState->discard[0][postState->discardCount[0] - 1] == 4);
        assert(memcmp(preState, postState, sizeof(struct gameState)) != 0);


        memcpy(preState->discard, postState->discard, sizeof(int) * 2000);
        preState->discardCount[0] = postState->discardCount[0];
        preState->supplyCount[targetCard] = postState->supplyCount[targetCard];

        assert(postState->numBuys == 0);
        //assert(postState->phase == 2);
        

        preState->phase = postState->phase;
        preState->numBuys = postState->numBuys;
                        
        if (gameStateComparer(preState, postState) != 0)
            assert(memcmp(preState, postState, sizeof(struct gameState)) == 0);
            



        /* 
         * Third test: would buyCard still buy a card even if it doesn't have enough
         * money? To test, buy adventurer at the beginning of the game. Shouldn't
         * be able to buy it.
         */

        
        targetCard = 7;
        initializeGame((int)floor(Random() * (MAX_PLAYERS - 2)) + 2, cards, seed, preState);
        preState->hand[0][0] = 0;
        preState->hand[0][1] = 0;
        preState->hand[0][2] = 0;
        preState->hand[0][3] = 0;
        preState->hand[0][4] = 0;
        memcpy(postState, preState, sizeof(struct gameState));

        buyCard(targetCard, postState);
        cout << "gameState comparison #4: after buyCard for adventurer without enough money" << endl;
        gameStateComparer(preState, postState);
        assert(memcmp(preState, postState, sizeof(struct gameState)) == 0);

        endTurn(postState);
        assert(postState->numActions == 1);
        assert(postState->numBuys == 1);
        assert(postState->phase < 2);
        

        /*
        if (repeats == 0) {
            cout << "More repetitions (-1 to quit): ";
            cin >> repeatCount;
            repeats = strtol(repeatCount.c_str(), NULL, 10);
        }*/

    }
    
    
    cout << "Tests complete." << endl;

    free(preState);
    free(postState);

    return 0;
}


void printCurrentStats(gameState *state) {
    cout << "Coins: " << state->coins << ", Actions: " << state->numActions << ", Buys: " << state->numBuys << endl;
    cout << "Score: " << scoreFor(state->whoseTurn, state) << endl;
}


int gameStateComparer (gameState *a, gameState *b) 
{
    result.clear();
    
    if (a->coins != b->coins)
        result.append("coins unequal\n");

    if (memcmp(a->supplyCount, b->supplyCount, sizeof(int) * 27) != 0)
        result.append("supplyCount unequal\n");
    if (memcmp(a->embargoTokens, b->embargoTokens, sizeof(int) * 27) != 0)
        result.append("embargoTokens unequal\n");

    if (a->numActions != b->numActions)
        result.append("numActions unequal\n");

    if (memcmp(a->playedCards, b->playedCards, sizeof(int) * 500) != 0)
        result.append("playedCards unequal\n");
    if (a->playedCardCount != b->playedCardCount) 
        result.append("playedCardCount unequal\n");
    if (a->numBuys != b->numBuys) 
        result.append("numBuys unequal\n");
    
    if (a->outpostPlayed != b->outpostPlayed) 
        result.append("outpostPlayed unequal\n");

    if (a->outpostTurn != b->outpostTurn) 
        result.append("outpostTurn unequal\n");

    if (a->phase != b->phase)
        result.append("phase unequal\n");

    if (a->whoseTurn != b->whoseTurn)
        result.append("whoseTurn unequal\n");



    
    if (memcmp(a->deckCount, b->deckCount, sizeof(int) * 4) != 0)
        result.append("discardCount unequal\n");
    if (memcmp(a->deck, b->deck, sizeof(int) * 2000) != 0)
        result.append("decks unequal\n");


    if (memcmp(a->discard, b->discard, sizeof(int) * 2000) != 0)
        result.append("discard unequal\n");
    if (memcmp(a->discardCount, b->discardCount, sizeof(int) * 4))
        result.append("discardCount unequal\n");


    if (memcmp(a->hand, b->hand, sizeof(int) * 2000) != 0)
        result.append("hand unequal\n");
    if (memcmp(a->handCount, b->handCount, sizeof(int) * 4) != 0)
        result.append("handCount unequal\n");
    
    cout << "Comparison results: " << (result.compare("") != 0?"Unequal":"Equal") << endl << result << endl;

    if (result.compare("") != 0)
        return -1;
    else
        return 0;
}

