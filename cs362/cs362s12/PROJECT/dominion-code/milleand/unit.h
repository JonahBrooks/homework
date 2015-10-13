#ifndef UNIT_H
#define UNIT_H

#include "GinkoTest.h"

void TestCompare(GinkoTest* tc);
void TestInitializeGame(GinkoTest* tc);
void TestShuffle(GinkoTest* tc);
void TestPlayCard(GinkoTest* tc);
void TestBuyCard(GinkoTest* tc);
void TestNumHandCards(GinkoTest* tc);
void TestHandCard(GinkoTest* tc);
void TestSupplyCount(GinkoTest* tc);
void TestFullDeckCount(GinkoTest* tc);
void TestWhoseTurn(GinkoTest* tc);
void TestEndTurn(GinkoTest* tc);
void TestIsGameOver(GinkoTest* tc);
void TestScoreFor(GinkoTest* tc);
void TestGetWinners(GinkoTest* tc);

#endif // UNIT_H
