#include "suite.h"

#include "GinkoTest.h"
#include "unit.h"

void RunAllTests(void)
{
    GinkoString *output = newGinkoString();
    GinkoSuite* suite = newGinkoSuite();
	
    SUITE_ADD_TEST(suite, TestCompare);
	SUITE_ADD_TEST(suite, TestInitializeGame);
	SUITE_ADD_TEST(suite, TestShuffle);
	SUITE_ADD_TEST(suite, TestPlayCard);
	SUITE_ADD_TEST(suite, TestBuyCard);
	SUITE_ADD_TEST(suite, TestNumHandCards);
	SUITE_ADD_TEST(suite, TestHandCard);
	SUITE_ADD_TEST(suite, TestSupplyCount);
	SUITE_ADD_TEST(suite, TestFullDeckCount);
	SUITE_ADD_TEST(suite, TestWhoseTurn);
	SUITE_ADD_TEST(suite, TestEndTurn);
	SUITE_ADD_TEST(suite, TestIsGameOver);
	SUITE_ADD_TEST(suite, TestScoreFor);
	SUITE_ADD_TEST(suite, TestGetWinners);

    ginkoSuiteRun(suite);
    ginkoSuiteSummary(suite, output);
    ginkoSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
}
