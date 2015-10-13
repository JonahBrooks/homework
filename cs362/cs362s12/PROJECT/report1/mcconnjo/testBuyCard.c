#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"


#define NUMBER_OF_TESTS 1000


/*
 * <Summary></Summary>
 * <Param name="post">The gamestate that is passed in.</Param>
 * <Param name="p">the current player</Param>
 * <Return><Return>
 */
int buyCardTest(int p,int supplyPos, struct gameState * post );


int main(int c, char ** args){
    int n, i, p, card, number_of_active_cards;
    int k[] = {copper,silver,gold,curse,estate,duchy,province,adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    number_of_active_cards = 18;
    struct gameState G;
    SelectStream(2);
    PutSeed(3);
    
     
    for(n = 0; n < NUMBER_OF_TESTS; ++n){
        for(i = 0; i < sizeof(struct gameState); ++i){
            ((char *)&G)[i] = floor(Random() * 256);
        }
        card = k[(int)floor( Random() * number_of_active_cards )];
        p = floor(Random() * 2);
        G.numBuys = floor(Random() * 5);
        G.coins = floor(Random() * 20);
        G.supplyCount[card] = floor(Random() * 20);
        G.whoseTurn = p;
        G.discardCount[p] = floor(Random() * MAX_DECK);
        buyCardTest(p, card, &G);
    }
    
    printf("All random Tests OK\n");

    //Unit tests for edge cases
    
    return 0;
} 


int buyCardTest(int p, int supplyPos,  struct gameState * post){
    struct gameState pre;
    memcpy(&pre, post, sizeof(struct gameState));
    
    int return_value;

    printf("p:%d supplyPos:%d coins:%d supplyCount:%d\n", p, supplyPos, pre.coins,pre.supplyCount[supplyPos]); 

    return_value = buyCard( supplyPos, post);
    //the gamestate should only change if that card is still in play and if the user has money 
    if(pre.supplyCount[supplyPos] > 0 
        && pre.numBuys > 0 
        && (pre.coins - getCost(supplyPos)) >= 0){

        printf("here\n");
        pre.coins -= getCost(supplyPos);
        pre.supplyCount[supplyPos]--;
        pre.discard[p][pre.discardCount[p]++] = supplyPos;
        pre.numBuys--; 
         
    }
    
    assert(return_value == 0);
    
    assert(memcmp(&pre, post, sizeof(struct gameState)) == 0);
    return 0;
}
