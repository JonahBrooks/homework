#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"


#define TESTS 5000

//I have gone through and commented off unecessary "noisy" code to clean up the output

int buyCardTest(int p, int supplyPos,  struct gameState * post){
    struct gameState pre;
    memcpy(&pre, post, sizeof(struct gameState));
    
    int flag, eDecSupply = 0, eDecBuy = 0, eRedCoin = 0;

    printf("XXXXXXXXXXXXXXXXXX--START TEST--XXXXXXXXXXXXXXXXXX\n");
    printf("Player:%d Buying Card:%d Cost:%d Coins:%d Pre Supply Count:%d\n", p, supplyPos, getCost(supplyPos), pre.coins,pre.supplyCount[supplyPos]); 

    flag = buyCard( supplyPos, post);

    printf("Player:%d Post Coins:%d Post Supply Count:%d\n", p, post->coins, post->supplyCount[supplyPos]);

    if(pre.supplyCount[supplyPos] > 0 && pre.numBuys > 0 && (pre.coins - getCost(supplyPos)) >= 0){

        if(post->supplyCount[supplyPos] != (pre.supplyCount[supplyPos] - 1)){//There should be one less card in supply
            eDecSupply = 1;
            printf("DecSupplyError, ");
        }
        if(post->numBuys != (pre.numBuys - 1)){ //Number of buys should be decremented
            eDecBuy = 1;
            printf("DecBuyError, ");
        }
        if(post->coins != (pre.coins - getCost(supplyPos))){//The coins should be reduced by the cost
            eRedCoin = 1;
            printf("ReduceCoinsError\n");
        }
		
    }
    
    if(eDecSupply == 1 && eDecBuy == 0 && eRedCoin ==0){
	return 1;
    }
    if(eDecSupply == 1 && eDecBuy == 1 && eRedCoin ==0){
	return 2;
    }
    if(eDecSupply == 1 && eDecBuy == 1 && eRedCoin ==1){
	return 3;
    }
    if(eDecSupply == 0 && eDecBuy == 1 && eRedCoin ==0){
	return 4;
    }
    if(eDecSupply == 0 && eDecBuy == 1 && eRedCoin ==1){
	return 5;
    }
    if(eDecSupply == 0 && eDecBuy == 0 && eRedCoin ==1){
	return 6;
    }
    if(eDecSupply == 1 && eDecBuy == 0 && eRedCoin ==1){
	return 7;
    }
    
    return 0;
}


int main(int c, char ** args){
    int n, i, p, supplyPos, numPiles, e;
    int eDecSupply = 0, eDecBuy = 0, eRedCoin = 0, Good = 0;
    int k[] = {copper, silver, gold, curse, estate, duchy, province, adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    numPiles = 17;
    struct gameState G;
    SelectStream(2);
    PutSeed(3);
    
    for(n = 0; n < TESTS; ++n){ //setup a random state and Test
        for(i = 0; i < sizeof(struct gameState); ++i){
            ((char *)&G)[i] = floor(Random() * 256);
        }
        supplyPos = k[(int)floor( Random() * numPiles )];
        p = floor(Random() * 2);
        G.numBuys = floor(Random() * 5);
        G.coins = floor(Random() * 20);
        G.supplyCount[supplyPos] = floor(Random() * 20);
        G.whoseTurn = p;
        G.discardCount[p] = floor(Random() * MAX_DECK);
        printf("Test #%d\n",n);
        e = buyCardTest(p, supplyPos, &G);
	switch(e){
	  case 0:
	  {  Good = Good + 1;
            break; }
	  case 1:
          {  eDecSupply = eDecSupply + 1;
            break; }
	  case 2:
          {  eDecSupply = eDecSupply + 1;
	    eDecBuy = eDecBuy + 1;
            break; }
	  case 3:
          {  eDecSupply = eDecSupply + 1;
	    eDecBuy = eDecBuy + 1;
	    eRedCoin = eRedCoin + 1;
            break; }
	  case 4:
          {  eDecBuy = eDecBuy + 1;
            break; }
	  case 5:
          {  eDecBuy = eDecBuy + 1;
	    eRedCoin = eRedCoin + 1;
            break; }
	  case 6:
          {  eRedCoin = eRedCoin + 1;
            break; }
	  case 7:
          {  eDecSupply = eDecSupply + 1;
 	    eRedCoin = eRedCoin + 1;}
         }
    }
    
    printf("Good Tests:%d \n Supply not decremented Errors:%d \n Buys not decemented Errors:%d \n Coins not reduced Errors:%d \n",Good,eDecSupply,eDecBuy,eRedCoin);
    
    return 0;
} 
