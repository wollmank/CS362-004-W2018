#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define YELLOW "\x1b[33m"

	
	/******************Adventurer Tests**********************/
void adventurerTests()
{	
				/*****Random Testing***********/
	int i,n,p, result;
	int timesMOff = 0, totalMOff = 0, timesDOff = 0, totalDOff = 0, timesTOff = 0, totalTOff;
	int treasureTest = 1, movedTest = 1, discardTest = 1, shuffleTest = 1, shuffleCalled = 0;
	int numTests = 20000;
	float avgMOff, avgDOff, avgTOff;
	
	struct gameState G, pre;

	//seed random number generator
	SelectStream(54);
	PutSeed(414);
	
	printf (YELLOW"\nBeginning test of adventurer\n"RESET);
	
	for(n = 0; n < numTests; n++)
	{
		//set game to random state
		for(i = 0; i < sizeof(struct gameState); i++)
		{
			((char*)&G)[i] = floor(Random() * 256);
		}
		
		p = floor(Random() * 2);
		G.whoseTurn = p;
		G.deckCount[p] = floor(Random() * MAX_DECK);
		
		G.discardCount[p] = floor(Random() * MAX_DECK);
		
		G.handCount[p] = floor(Random() * MAX_HAND);
	
		
		//assign random cards to deck, discard, and hand piles
		for(i = 0; i < G.deckCount[p]; i++)
		{
			G.deck[p][i] = floor(Random() * 26);
		}
		
		for(i = 0; i < G.discardCount[p]; i++)
		{
			G.discard[p][i] = floor(Random() * 26);
		}
		
		for(i = 0; i < G.handCount[p]; i++)
		{
			G.hand[p][i] = floor(Random() * 26);
		}
		
		//put at least 2 treasures in play to avoid seg fault
		if(G.deckCount[p] > 0)
		{
			G.deck[p][0] = copper;
			G.deck[p][G.deckCount[p] - 1] = copper;
		}
		else if(G.discardCount[p] > 0)
		{
			G.discard[p][0] = copper;
			G.discard[p][G.discardCount[p] - 1] = copper;
		}
		
		//create a pre call copy of gameState
		memcpy(&pre, &G, sizeof(struct gameState));
		
		
		
		if(pre.deckCount[p] > 0)
		{
			result = adventurer_card(&G);
			
			//test that correct number of cards are being moved to hand
			if(pre.handCount[p] + 2 != G.handCount[p])
			{
				totalMOff += abs(abs(G.handCount[p] - pre.handCount[p]) - 2);
				timesMOff++;
				movedTest = 0;
			}
			else
			{
				movedTest &= movedTest;
			}
			
			//test that two treasures are added to player's hand
			int GTreasure = 0, preTreasure = 0;
			
			for(i = 0; i < G.handCount[p]; i++)
			{
				if(G.hand[p][i] == copper || G.hand[p][i] == silver || G.hand[p][i] == gold)
				{
					GTreasure++;
				}
			}
			
			for(i = 0; i < pre.handCount[p]; i++)
			{
				if(pre.hand[p][i] == copper || pre.hand[p][i] == silver || pre.hand[p][i] == gold)
				{
					preTreasure++;
				}
			}
			
			if(preTreasure + 2 != GTreasure)
			{
				totalTOff += abs((2 + preTreasure) - GTreasure);
				timesTOff++;
				treasureTest = 0;
			}
			
			else
			{
				treasureTest &= treasureTest;
			}

			//test that proper number of cards get discarded
			
			int numDiscarded = pre.deckCount[p] - (G.deckCount[p] + 2);
			int expectedDiscarded = G.discardCount[p] - pre.discardCount[p];
			
			if(numDiscarded != expectedDiscarded)
			{
				totalDOff += abs(expectedDiscarded - numDiscarded);
				timesDOff++;
				discardTest = 0;
			}
			
			else
			{
				discardTest &= discardTest;
			}
		}
		
		//test that shuffle gets called at proper time
		else if(pre.discardCount[p] > 0 && pre.deckCount[p] == 0)
		{
			adventurer_card(&G);
			
			shuffleCalled = 1;//called flag
			
			if(G.discardCount[p] != 0)
			{
				shuffleTest = 0;
			}
			
			else
			{
				shuffleTest &= shuffleTest;
			}
		}
	
	}
	
	if(movedTest)
	{
		printf(GREEN"Passed test: Adventurer adds two cards to player's hand\n"RESET);
	}
	else
	{
		avgMOff = (float)totalMOff / (float)timesMOff;
		
		printf(RED"Failed test: Adventurer card not adding proper number of cards to player's hand. Off by an average of %5.2f cards."RESET, 
				avgMOff);
		printf(RED" Out of %d tests, %d failed.\n"RESET, numTests, timesMOff);
	}
	
	if(treasureTest)
	{
		printf(GREEN"Passed test: Adventurer adding correct number of treasure to player's hand.\n"RESET);
	}
	else
	{
		avgTOff = (float)totalTOff / (float) timesTOff;
		printf(RED"Failed test: Adventurer not adding correct number of treasure to player's hand. Off by an average of %5.2f cards.", avgTOff);
		printf(RED" Out of %d tests, %d failed.\n"RESET, numTests, timesTOff);
	}
	
	if(discardTest)
	{
		printf(GREEN "Passed test: Adventurer discarding correct number of cards.\n"RESET);
	}
	else
	{
		avgDOff = (float)totalDOff / (float)timesDOff;
		printf(RED "Failed test: Adventurer not discarding proper number of cards. Off by an average of %5.2f cards.", avgDOff);
		printf(RED" Out of %d tests, %d failed.\n"RESET, numTests, timesDOff);
	}
	
	if(shuffleCalled)
	{
		if(shuffleTest)
		{
			printf(GREEN "Passed test: Adventurer properly calling shuffle\n"RESET);
		}
		else
		{
			printf(RED "Failed test: Adventurer not calling shuffle at proper time\n"RESET);
		}
	}
	
				/***************Unit Testing***********/
				
	int treasureCount = 0;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    int player = 0;
	struct gameState G2;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	//put player's hand and deck in known state
	for(i = 0; i < MAX_DECK; i++){
		G.hand[player][i] = G.deck[player][i] = 0;
	}
	
	//make a copy for testing
	memcpy(&G2, &G, sizeof(struct gameState));
	
	//add coppers at known locations
	G2.deck[player][0] = G2.deck[player][1] = 4;
	
	cardEffect(adventurer, choice1, choice2, choice3, &G2, handpos, &bonus);
	
	
	//check if handCount increases by 2 and gains copper
	if(G2.handCount[player] == G.handCount[player] + 2)
		printf(GREEN "Passed test: Adventurer accurately updates number of cards in hand!\n" RESET);
	else
		printf(RED "Failed test: Adventurer not accurately updating hand count. Expecting 7 : Actual = %d\n"RESET, G2.handCount[player]);
	
	int preTreasureCount = 0;
	
	for(i = 0; i < G2.handCount[player]; i++){
		if(G.hand[player][i] == copper){
			preTreasureCount++;
		}
	}
	
	int postTreasureCount = 0;
	
	for(i = 0; i < G2.handCount[player]; i++){
		if(G2.hand[player][i] == copper){
			postTreasureCount++;
		}
	}
	
	if((postTreasureCount - preTreasureCount) == 2){
		printf(GREEN "Passed test: Adventurer accurately adds 2 treasure to player's hand!\n"RESET);
	}
	else{
		printf(RED "Failed test: Adventurer not accurately adding 2 treasure to player's hand. Expecting 2 : Actual = %d\n"RESET, treasureCount);
	}
	
	//reset G2
	memcpy(&G2, &G, sizeof(struct gameState));
	
	//test that cards drawn while looking for treasure gets added to discard pile
	
	//put copper in first and last position of deck
	G2.deck[player][0] = G2.deck[player][4] = 4;
	
	int before = G2.deckCount[player] + G2.discardCount[player];
	
	cardEffect(adventurer, choice1, choice2, choice3, &G2, handpos, &bonus);
	
	int after = G2.deckCount[player] + G2.discardCount[player] + 2; //+2 to account for the 2 treasure cards added to hand
	if(before == after){
		printf(GREEN "Passed Test: Adventurer accurately moves cards from deck to discard pile!\n"RESET);
	}
	else{
		printf(RED "Failed Test: Adventurer does not accurately move cards from deck to discard pile. Before = %d After = %d\n"RESET, before, after);
	}
	
	//reset G2
	memcpy(&G2, &G, sizeof(struct gameState));
	
	//test that adventurer calls reshuffle when needed
	
	//set deck and discard sizes
	G2.deckCount[player] = 0;
	G2.discardCount[player] = 5;
	
	//fill discard pile
	for(i = 0; i < 5; i++){
		G2.discard[player][i] = 0;
	}
	
	//put coppers in discard pile
	G2.discard[player][3] = G2.discard[player][4] = 4;
	 
	cardEffect(adventurer, choice1, choice2, choice3, &G2, handpos, &bonus);
	
	//deckCount should be discardCount - 2 coppers put in hand
	if(G2.deckCount[player] == 3){
		printf(GREEN "Passed test: Adventurer calls shuffle at correct time!\n"RESET);
	}
	
	else{
		printf(RED "Failed test: Adventurer not calling shuffle at right time. Deck count expected at 3 : Actual = %d\n"RESET, G2.deckCount[player]);
	}
	
	printf(YELLOW"Finished testing adventurer\n\n"RESET);
}
						/*******************************Smithy Tests**********************/

void smithyTests()
{						
									/***************Random Testing***********/
									
	
	int i,n,p, result;
	int hcTest = 1, playedCardTest = 1;
	int timesHCOff = 0, totalHCOff = 0, timesDOff = 0;
	int numTests = 20000;
	float avgHCOff;
	
	struct gameState G, pre;
	int ss = random() % 255 + 1;
	int ps = random () % 1000 + 1;
	
	//seed random number generator
	SelectStream(ss);
	PutSeed(ps);
	
	printf (YELLOW"\nBeginning test of smithy\n"RESET);
	
	for(n = 0; n < numTests; n++)
	{
		//set game to random state
		for(i = 0; i < sizeof(struct gameState); i++)
		{
			((char*)&G)[i] = floor(Random() * 256);
		}
		
		p = floor(Random() * 2);
		G.whoseTurn = p;
		G.deckCount[p] = floor(Random() * MAX_DECK);
		
		G.discardCount[p] = floor(Random() * MAX_DECK);
		
		G.handCount[p] = floor(Random() * MAX_HAND);
	
		G.playedCardCount = 0;
		
		//assign random cards to deck, discard, and hand piles
		for(i = 0; i < G.deckCount[p]; i++)
		{
			G.deck[p][i] = floor(Random() * 26);
		}
		
		for(i = 0; i < G.discardCount[p]; i++)
		{
			G.discard[p][i] = floor(Random() * 26);
		}
		
		for(i = 0; i < G.handCount[p]; i++)
		{
			G.hand[p][i] = floor(Random() * 26);
		}
		
		int handPos = floor(Random() * G.handCount[p]);
		
		memcpy(&pre, &G, sizeof(struct gameState));
		
		result = smithy_card(&G, handPos);
		
		//test that Smithy draws the correct number of cards
		
		if(pre.handCount[p] + 2 != G.handCount[p])
		{
			hcTest = 0;
			totalHCOff = abs(G.handCount[p] - (pre.handCount[p] + 2));
			timesHCOff++;
		}
		
		else
		{
			hcTest &= hcTest;
		}
		
		//test that Smithy correctly discarding played card
		
		if(pre.hand[p][handPos] != G.playedCards[G.playedCardCount - 1])
		{
			playedCardTest = 0;
			timesDOff++;
		}
		else
		{
			playedCardTest &= playedCardTest;
		}
	}
	
	if(hcTest)
	{
		printf(GREEN "Passed test: Smithy adding correct number of cards to player's hand\n"RESET);
	}
	else
	{
		avgHCOff = (float)totalHCOff / (float)timesHCOff;
		
		printf(RED "Failed test: Smithy not adding correct number of cards to player's hand. Off by an average of %5.2f cards. ",avgHCOff);
		printf("Out of %d tests, %d failed\n"RESET, numTests, timesHCOff);
	}
	
	if(playedCardTest)
	{
		printf(GREEN "Passed test: Smithy correctly discarding played card\n."RESET);
	}
	else
	{
		printf(RED "Failed test: Smithy not correclty discarding played card. Out of %d tests %d failed.\n"RESET, numTests, timesDOff);
	}
	
								/*******************Unit Testing***********/
								
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    int player = 0;
	struct gameState G2;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	//make a copy for testing
	memcpy(&G2, &G, sizeof(struct gameState));
	
	cardEffect(smithy, choice1, choice2, choice3, &G2, handpos, &bonus);
	
	//check if handCount increases by +3 for new cards -1 for discard played card
	if(G2.handCount[player] == 7)
		printf(GREEN "Passed test: Smithy accurately updates number of cards in hand!\n" RESET);
	else
		printf(RED "Failed test: Smithy not accurately updating hand count. Expecting 7 : Actual = %d\n"RESET, G2.handCount[player]);
	
	memcpy(&G2, &G, sizeof(struct gameState));
	
	//check that smithy discards card accurately
	int before = G2.playedCardCount;
	
	cardEffect(smithy, choice1, choice2, choice3, &G2, handpos, &bonus);
	
	int after = G2.playedCardCount - 1; //subtract 1 for card that was playedCardCount
	
	if(before == after)
		printf(GREEN "Passed test: Smithy accurately discards played card!\n" RESET);
	else
		printf(RED "Failed test: Smithy not accurately discarding played card. Before = %d After = %d\n"RESET, before, after);
	
	printf (YELLOW"Finished testing Smithy\n\n"RESET);
}
	
				/****************************Council Room Tests**********************/

void council_roomTests()
{				
							/***************Random Testing***********/
							
	int i,n,p, j, result;
	int othersTest = 1, hcTest = 1, buysTest = 1, playedCardTest = 1;
	int timesOOff = 0, totalOOff = 0, timesHCOff = 0, totalHCOff = 0, timesBOff = 0, totalBOff = 0, timesDOff = 0;
	int numTests = 20000, numPlayers = 3, othersTested = 0;
	float avgOOff, avgHCOff, avgBOff;
	
	struct gameState G, pre;
	int ss = random() % 255 + 1;
	int ps = random () % 1000 + 1;
	
	//seed random number generator
	SelectStream(ss);
	PutSeed(ps);
	
	printf (YELLOW"\nBeginning test of council room\n"RESET);
	
	for(n = 0; n < numTests; n++)
	{
		//set game to random state
		for(i = 0; i < sizeof(struct gameState); i++)
		{
			((char*)&G)[i] = floor(Random() * 256);
		}
		
		G.numPlayers = numPlayers;
		
		G.playedCardCount = 0;
		
		//fill in player data with random values
		for(j = 0; j < numPlayers; j++)
		{
			G.deckCount[j] = floor(Random() * MAX_DECK);
			
			G.discardCount[j] = floor(Random() * MAX_DECK);
			
			G.handCount[j] = floor(Random() * MAX_HAND); 
		
			//assign random cards to deck, discard, and hand piles
			for(i = 0; i < G.deckCount[j]; i++)
			{
				G.deck[j][i] = floor(Random() * 26);
			}
			
			for(i = 0; i < G.discardCount[j]; i++)
			{
				G.discard[j][i] = floor(Random() * 26);
			}
			
			for(i = 0; i < G.handCount[j]; i++)
			{
				G.hand[j][i] = floor(Random() * 26);
			}
		}
		
		p = floor(Random() * numPlayers);
		G.whoseTurn = p;
		int handPos = floor(Random() * G.handCount[p]);
		
		G.numBuys = 1;
		
		//create a pre call copy of gameState
		memcpy(&pre, &G, sizeof(struct gameState));
		
		result = council_room_card(&G, handPos);
		
		//test if player gets +4 cards added to hand -1 for discarding card played
		if(G.handCount[p] != pre.handCount[p] + 3)
		{
			hcTest = 0;
			totalHCOff += abs((pre.handCount[p] + 3) - G.handCount[p]);
			timesHCOff++;
		}
		
		else
		{
			hcTest &= hcTest;
		}
		
		//test if council_room updates player's number of buys correctly
		if(G.numBuys != 2)
		{
			buysTest = 0;
			totalBOff += abs(2 - G.numBuys);
			timesBOff++;
		}
		
		else
		{
			buysTest &= buysTest;
		}
	
		//test that council_room accurately updates other player's hand count
		
		for(i = 0; i < numPlayers; i++)
		{
			if(i != p)
			{
				othersTested++;
				if(pre.handCount[i] != G.handCount[i] - 1)
				{
					totalOOff += abs((G.handCount[i] + 1) - pre.handCount[i]);
					timesOOff++;
					othersTest = 0;
				}
				else
				{
					othersTest &= othersTest;
				}
			}
		}
		
		//test if played card is being moved to played card pile
		
		if(pre.hand[p][handPos] != G.playedCards[G.playedCardCount - 1])
		{
			playedCardTest = 0;
			timesDOff++;
		}
		else
		{
			playedCardTest &= playedCardTest;
		}
		
		
	}
	 
	//print test results
	if(hcTest)
	{
		printf(GREEN "Passed test: Council Room draws correct number of cards!\n" RESET);
	}
	else
	{
		avgHCOff = (float)totalHCOff / (float)timesHCOff;
		printf(RED "Failed test: Council Room not accurately updating hand count. Off by an average of %5.2f cards.", avgHCOff);
		printf(" Out of %d tests %d failed\n"RESET, numTests, timesHCOff);
	}
	
	if(buysTest)
	{
		printf(GREEN "Passed test: Council Room updates number of buys correctly!\n"RESET);
	}
	
	else
	{
		avgBOff = (float)totalBOff / (float)timesBOff;
		printf(RED "Failed test: Council Room not accurately updating number of buys. Off by an average of %5.2f cards.", avgBOff);
		printf(" Out of %d tests %d failed\n"RESET, numTests, timesBOff);
	}
		
	if(othersTest)
	{
		printf(GREEN "Passed test: Council Room correctly draws a new card for each player!\n"RESET);
	}
	
	else
	{
		avgOOff = (float)totalOOff / (float)timesOOff;
		printf(RED "Failed test: Council Room not accurately updating other player's hand count. Off by an average of %5.2f cards.", avgOOff);
		printf(" Out of %d tests %d failed\n"RESET, othersTested, timesOOff);
	}
	
	if(playedCardTest)
	{
		printf(GREEN "Passed test: Council Room correctly discarding played card\n."RESET);
	}
	else
	{
		printf(RED "Failed test: Council Room not correclty discarding played card. Out of %d tests %d failed.\n"RESET, numTests, timesDOff);
	}
	
							/******************Unit Testing***********/
							
	int passed = 1;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    numPlayers = 3;
    int player = 0;
	struct gameState G2;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	//make a copy for testing
	memcpy(&G2, &G, sizeof(struct gameState));
	
	cardEffect(council_room, choice1, choice2, choice3, &G2, handpos, &bonus);
	
	//test draw 4 cards
	//check if handCount increases by +4 for new cards -1 for discard played card
	if(G2.handCount[player] == 8)
		printf(GREEN "Passed test: Council Room draws correct number of cards!\n" RESET);
	else
		printf(RED "Failed test: Council Room not accurately updating hand count. Expecting 8 : Actual = %d\n"RESET, G2.handCount[player]);
	
	memcpy(&G2, &G, sizeof(struct gameState));
	
	//check if council_room increases number of buys accurately
	
	cardEffect(council_room, choice1, choice2, choice3, &G2, handpos, &bonus);
	
	if(G2.numBuys == 2)
		printf(GREEN "Passed test: Council Room updates number of buys correctly!\n"RESET);
	else
		printf(RED "Failed test: Council Room not updating number of buys correctly. Expected %d : Actual = %d\n"RESET, 2, G2.numBuys);
	
	memcpy(&G2, &G, sizeof(struct gameState));
	
	//test if council_room updates other player's hands correctly
	
	cardEffect(council_room, choice1, choice2, choice3, &G2, handpos, &bonus);
	
	for(player = 1; player < G2.numPlayers; player++){
		if(G2.handCount[player] != 1){
		   printf(RED "Test failed: Council Room not updating player %d's hand correctly. Expected 1 cards, actual = %d\n"RESET, player, G2.handCount[player]);
		   passed = 0;
		}
	}
	
	if(passed){
		printf(GREEN "Passed test: Council Room correclty draws a new card for each player!\n"RESET);
	}
		 
	printf (YELLOW"Finished testing Council Room\n\n"RESET);
	
}

void sea_hagTest()
{
	int i;
    int deckpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0, passed = 1;
    int seed = 1000;
    int numPlayers = 3;
    int player = 0;
	struct gameState G, G2;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	
	//set player's decks to known state
	G.deckCount[1] = G.deckCount[2] = G.discardCount[player] = G.discardCount[1] = G.discardCount[2] = 5;
	
	for(i = 0; i < 5; i++){
		G.hand[player][i] = G.hand[1][i] = G.hand[2][i] = gold;
		G.deck[player][i] = G.deck[1][i] = G.deck[2][i] = sea_hag;
		G.discard[player][i] = G.discard[1][i] = G.discard[2][i] = smithy;
	}
	
	//make a copy for testing
	memcpy(&G2, &G, sizeof(struct gameState));
	
	printf (YELLOW"\nBeginning test of Sea Hag\n"RESET);
	
	//test if sea hag is discarding from other players' decks properly
	
	int before1 = G2.deckCount[1];
	int before2 = G2.deckCount[2];
	
	cardEffect(sea_hag, choice1, choice2, choice3, &G2, deckpos, &bonus);
	
	int after1 = G2.deckCount[1];
	int after2 = G2.deckCount[2];
	
	//sea_hag should remove a card from a player's deck and add a curse card
	//so before card count should equal after card count
	if(before1 != after1){
		printf(RED "Failed test: Sea Hag doesn't correctly discard top card from player %d's deck.\n"RESET, 1);
		passed = 0;
	}
	
	if(before2 != after2){
		printf(RED "Failed test: Sea Hag doesn't correctly discard top card from player %d's deck.\n"RESET, 2);
		passed = 0;
	}
	
	if(passed){
		printf(GREEN "Passed test: Sea Hag correclty discards top card from players' decks!\n"RESET);
	}
	
	passed = 1;
	
	//test if sea hag places curse on top of players 
	
	//reset G2
	memcpy(&G2, &G, sizeof(struct gameState));
	
	cardEffect(sea_hag, choice1, choice2, choice3, &G2, deckpos, &bonus);
	
	if(G2.deck[1][G2.deckCount[1]  - 1] != curse){
		printf(RED "Failed test: Sea Hag not placing curse on top of player %d's deck\n" RESET, 1);
		passed = 0;
	}
	
	if(G2.deck[2][G2.deckCount[2] - 1] != curse){
		printf(RED "Failed test: Sea Hag not placing curse on top of player %d's deck\n" RESET, 2);
		passed = 0;
	}
	
	if(passed){
		printf(GREEN "Passed test: Sea Hag correctly putting curse on top of other players' decks!\n"RESET);
	}
	
	passed = 1;
	
	//reset G2
	memcpy(&G2, &G, sizeof(struct gameState));
	
	//test if sea hag effects current player's hand
	int deckBefore = G2.deckCount[player];
	int discardBefore = G2.discardCount[player];
	
	cardEffect(sea_hag, choice1, choice2, choice3, &G2, deckpos, &bonus);
	
	int deckAfter = G2.deckCount[player];
	int discardAfter = G2.discardCount[player];
	
	if(deckBefore == deckAfter && discardBefore == discardAfter){
		printf(GREEN "Passed test: Sea Hag does not effect current player's cards!\n" RESET);
	}
	
	else
		printf(RED "Failed test: Sea Hag effects current player's cards.\n" RESET);
	
	printf(YELLOW "Finished testing Sea Hag.\n\n" RESET);
}

void unittest1()
{
	int player = 0;
	int default_card = curse;
	int	card,count, test_card;
	struct gameState G;
	memset(&G, 49, sizeof(struct gameState));
	
	//set player's hand to known state
	G.handCount[player] = 100;
	G.deckCount[player] = 200;
	G.discardCount[player] = MAX_DECK;
	
	for(card=0; card < 100; card++){
		G.hand[player][card] = default_card;
	}
	
	for(card=0; card < 200; card++){
		G.deck[player][card] = default_card;
	}
	
	for(card=0; card < MAX_DECK; card++){
		G.discard[player][card] = default_card;
	}
	
	printf (YELLOW"\nBeginning test of fullDeckCount()\n"RESET);
	
	//test for default card
	test_card = default_card;
	count = fullDeckCount(player, test_card, &G);
	if(count == MAX_DECK + 300) {
		printf(GREEN"Passed test counting default card\n"RESET);
	}
	else {
		printf(RED"Test failed counting default card. Expected %d found %d\n"RESET, (MAX_DECK + 300), count);
	}
	
	//test for card not present
	test_card = estate;
	count = fullDeckCount(player, test_card, &G);
	if(count == 0) {
		printf(GREEN"Passed test counting card not present\n"RESET);
	}
	else {
		printf(RED"Test failed counting card not present. Expected 0. Found %d\n"RESET, count); 
	}
	
	//test for card found only in hand
	test_card = treasure_map;
	G.hand[player][0] = test_card;
	count = fullDeckCount(player, test_card, &G);
	if(count == 1) {
		printf(GREEN"Passed test counting card found only in hand\n"RESET);
	}
	else {
		printf(RED"Test failed counting card found only in hand. Expected 1. Found %d\n"RESET, count);
	}
	
	//test for card found only in deck
	test_card = smithy;
	G.hand[player][0] = default_card;
	G.deck[player][0] = test_card;
	count = fullDeckCount(player, test_card, &G);
	if(count == 1) {
		printf(GREEN"Passed test counting card found only in deck\n"RESET);
	}
	else {
		printf(RED"Test failed counting card found only in deck. Expected 1. Found %d\n"RESET, count); 
	}
	
	//test for card found only in discard
	test_card = smithy;
	G.deck[player][0] = default_card;
	G.discard[player][0] = test_card;
	count = fullDeckCount(player, test_card, &G);
	if(count == 1) {
		printf(GREEN"Passed test counting card found only in discard\n"RESET);
	}
	else {
		printf(RED"Test failed counting card found only in discard. Expected 1. Found %d\n"RESET, count); 
	}
	
	//test boundary cases
	test_card = adventurer;
	G.discard[player][0] = default_card;
	G.hand[player][100 - 1] = test_card;
	G.deck[player][200 - 1] = test_card;
	G.discard[player][MAX_DECK - 1] = test_card;
	count = fullDeckCount(player, test_card, &G);
	if(count == 3) {
		printf(GREEN"Passed test counting card in last position\n"RESET);
	}
	else {
		printf(RED"Test failed counting card in last position. Expected 3. Found %d\n"RESET, count); 
	}
	
	printf(YELLOW"Finished testing fullDeckCount()\n\n"RESET);
}

void unittest2()
{
	int i, over;
	struct gameState G;
	memset(&G, 49, sizeof(struct gameState));

	//set supplyCount to known value
	for(i = 0; i < treasure_map + 1; i++){
		G.supplyCount[i] = MAX_DECK;
	}
	
	printf (YELLOW"\nBeginning test of isGameOver()\n"RESET);
	
	//check if game is over with full supply
	over = isGameOver(&G);
	if(over == 0){
		printf(GREEN "Passed test with fully supplied deck\n" RESET);
	}
	
	else{
		printf(RED "Failed test with fully supplied deck\n" RESET);
	}
	
	//check if game ends when province cards are empty
	G.supplyCount[province] = 0;
	
	over = isGameOver(&G);
	if(over == 1){
		printf(GREEN "Passed test when province is empty\n" RESET);
	}
	
	else{
		printf(RED "Failed test when province is empty\n" RESET);
	}
	
	//check if game ends when province is negative value
	G.supplyCount[province] = -1;
	
	over = isGameOver(&G);
	if(over == 1){
		printf(GREEN "Passed test when province is negative value\n" RESET);
	}
	
	else{
		printf(RED "Failed test when province is negative value\n" RESET);
	}
	
	//check if game ends when 1 supply pile runs out
	G.supplyCount[province] = MAX_DECK;
	G.supplyCount[smithy] = 0;
	
	over = isGameOver(&G);
	if(over == 0){
		printf(GREEN "Passed test when 1 supply pile runs out\n" RESET);
	}
	
	else{
		printf(RED "Failed test when 1 supply pile runs out\n" RESET);
	}
	
	//check if game ends when 2 supply piles run out
	G.supplyCount[smithy] = MAX_DECK;
	G.supplyCount[baron] = G.supplyCount[gardens] = 0;
	
	over = isGameOver(&G);
	if(over == 0){
		printf(GREEN "Passed test when 2 supply piles run out\n" RESET);
	}
	
	else{
		printf(RED "Failed test when 2 supply piles run out\n" RESET);
	}
	
	//check if game ends when 3 supply piles run out
	G.supplyCount[baron] = G.supplyCount[gardens] = MAX_DECK;
	G.supplyCount[gold] = G.supplyCount[adventurer] = G.supplyCount[minion] = 0;
	
	over = isGameOver(&G);
	if(over == 1){
		printf(GREEN "Passed test when 3 supply piles run out\n" RESET);
	}
	
	else{
		printf(RED "Failed test when 3 supply piles run out\n" RESET);
	}
	
	//check if game ends when supply piles become negative
	G.supplyCount[gold] = G.supplyCount[adventurer] = G.supplyCount[minion] = MAX_DECK;
	G.supplyCount[mine] = G.supplyCount[remodel] = G.supplyCount[duchy] = -1;
	
	over = isGameOver(&G);
	if(over == 1){
		printf(GREEN "Passed test when 3 supply piles become negative\n" RESET);
	}
	
	else{
		printf(RED "Failed test when 3 supply piles become negative\n" RESET);
	}
	
	//check boundary cases for supply piles
	G.supplyCount[gold] = G.supplyCount[adventurer] = G.supplyCount[minion] = MAX_DECK;
	G.supplyCount[curse] = G.supplyCount[treasure_map] = G.supplyCount[sea_hag] = 0;
	
	over = isGameOver(&G);
	if(over == 1){
		printf(GREEN "Passed test when first and last supply piles run out\n" RESET);
	}
	
	else{
		printf(RED "Failed test when first and last supply piles run out\n" RESET);
	}
	
	printf(YELLOW"Finished testing isGameOver()\n\n"RESET);
}

void unittest3()
{
	int score, card;
	int handMax = 100;
	int discardMax = 200;
	int deckMax = MAX_DECK;
	int player = 0;
	int default_card = gold;
	struct gameState G;
	memset(&G, 49, sizeof(struct gameState));
	
	//set player's hand to known state
	G.handCount[player] = handMax;
	G.discardCount[player] = discardMax;
	G.deckCount[player] = deckMax;
	
	for(card=0; card < handMax; card++){
		G.hand[player][card] = default_card;
	}
	
	for(card=0; card < discardMax; card++){
		G.discard[player][card] = default_card;
	}
	
	for(card=0; card < deckMax; card++){
		G.deck[player][card] = default_card;
	}
	
	printf (YELLOW"\nBeginning test of scoreFor()\n"RESET);
	
	//check score for default card
	score = scoreFor(player, &G);
	
	if(score == 0){
		printf(GREEN "Passed test for default value\n" RESET);
	}
	else{
		printf(RED "Failed test for default value. Expected score of 0 actual score = %d\n"RESET, score);
	}
	
	//test curse card detection in hand while also checking boundary cases
	G.hand[player][0] = curse;
	score = scoreFor(player, &G);
	
	if(score == -1){
		printf(GREEN "Passed test with curse card in first position in hand\n"RESET);
	}
	else{
		printf(RED "Failed test with curse card in first position in hand\n"RESET);
	}
	
	G.hand[player][0] = default_card;
	G.hand[player][handMax - 1] = curse;
	score = scoreFor(player, &G);
	
	if(score == -1){
		printf(GREEN "Passed test with curse card in last position in hand\n"RESET);
	}
	else{
		printf(RED "Failed test with curse card in last position in hand\n"RESET);
	}
	
	G.hand[player][handMax - 1] = default_card;

	//test curse card detection in discard while also checking boundary cases
	G.discard[player][0] = curse;
	score = scoreFor(player, &G);
	
	if(score == -1){
		printf(GREEN "Passed test with curse card in first position in discard\n"RESET);
	}
	else{
		printf(RED "Failed test with curse card in first position in discard\n"RESET);
	}
	
	G.discard[player][0] = default_card;
	G.discard[player][discardMax - 1] = curse;
	score = scoreFor(player, &G);
	
	if(score == -1){
		printf(GREEN "Passed test with curse card in last position in discard\n"RESET);
	}
	else{
		printf(RED "Failed test with curse card in last position in discard\n"RESET);
	}
	
	G.discard[player][discardMax - 1] = default_card;

	//test curse card detection in deck while also checking boundary cases
	G.deck[player][0] = curse;
	score = scoreFor(player, &G);
	
	if(score == -1){
		printf(GREEN "Passed test with curse card in first position in deck\n"RESET);
	}
	else{
		printf(RED "Failed test with curse card in first position in deck\n"RESET);
	}
	
	G.deck[player][0] = default_card;
	G.deck[player][deckMax - 1] = curse;
	score = scoreFor(player, &G);
	
	if(score == -1){
		printf(GREEN "Passed test with curse card in last position in deck\n"RESET);
	}
	else{
		printf(RED "Failed test with curse card in last position in deck\n"RESET);
	}
	
	G.deck[player][deckMax - 1] = default_card;
	
	//test estate card detection in hand while also checking boundary cases
	G.hand[player][0] = estate;
	score = scoreFor(player, &G);
	
	if(score == 1){
		printf(GREEN "Passed test with estate card in first position in hand\n"RESET);
	}
	else{
		printf(RED "Failed test with estate card in first position in hand\n"RESET);
	}
	
	G.hand[player][0] = default_card;
	G.hand[player][handMax - 1] = estate;
	score = scoreFor(player, &G);
	
	if(score == 1){
		printf(GREEN "Passed test with estate card in last position in hand\n"RESET);
	}
	else{
		printf(RED "Failed test with estate card in last position in hand\n"RESET);
	}
	
	G.hand[player][handMax - 1] = default_card;

	//test estate card detection in discard while also checking boundary cases
	G.discard[player][0] = estate;
	score = scoreFor(player, &G);
	
	if(score == 1){
		printf(GREEN "Passed test with estate card in first position in discard\n"RESET);
	}
	else{
		printf(RED "Failed test with estate card in first position in discard\n"RESET);
	}
	
	G.discard[player][0] = default_card;
	G.discard[player][discardMax - 1] = estate;
	score = scoreFor(player, &G);
	
	if(score == 1){
		printf(GREEN "Passed test with estate card in last position in discard\n"RESET);
	}
	else{
		printf(RED "Failed test with estate card in last position in discard\n"RESET);
	}
	
	G.discard[player][discardMax - 1] = default_card;

	//test estate card detection in deck while also checking boundary cases
	G.deck[player][0] = estate;
	score = scoreFor(player, &G);
	
	if(score == 1){
		printf(GREEN "Passed test with estate card in first position in deck\n"RESET);
	}
	else{
		printf(RED "Failed test with estate card in first position in deck\n"RESET);
	}
	
	G.deck[player][0] = default_card;
	G.deck[player][deckMax - 1] = estate;
	score = scoreFor(player, &G);
	
	if(score == 1){
		printf(GREEN "Passed test with estate card in last position in deck\n"RESET);
	}
	else{
		printf(RED "Failed test with estate card in last position in deck\n"RESET);
	}
	
	G.deck[player][deckMax - 1] = default_card;
	
	//test duchy card detection in hand while also checking boundary cases
	G.hand[player][0] = duchy;
	score = scoreFor(player, &G);
	
	if(score == 3){
		printf(GREEN "Passed test with duchy card in first position in hand\n"RESET);
	}
	else{
		printf(RED "Failed test with duchy card in first position in hand\n"RESET);
	}
	
	G.hand[player][0] = default_card;
	G.hand[player][handMax - 1] = duchy;
	score = scoreFor(player, &G);
	
	if(score == 3){
		printf(GREEN "Passed test with duchy card in last position in hand\n"RESET);
	}
	else{
		printf(RED "Failed test with duchy card in last position in hand\n"RESET);
	}
	
	G.hand[player][handMax - 1] = default_card;

	//test duchy card detection in discard while also checking boundary cases
	G.discard[player][0] = duchy;
	score = scoreFor(player, &G);
	
	if(score == 3){
		printf(GREEN "Passed test with duchy card in first position in discard\n"RESET);
	}
	else{
		printf(RED "Failed test with duchy card in first position in discard\n"RESET);
	}
	
	G.discard[player][0] = default_card;
	G.discard[player][discardMax - 1] = duchy;
	score = scoreFor(player, &G);
	
	if(score == 3){
		printf(GREEN "Passed test with duchy card in last position in discard\n"RESET);
	}
	else{
		printf(RED "Failed test with duchy card in last position in discard\n"RESET);
	}
	
	G.discard[player][discardMax - 1] = default_card;

	//test duchy card detection in deck while also checking boundary cases
	G.deck[player][0] = duchy;
	score = scoreFor(player, &G);
	
	if(score == 3){
		printf(GREEN "Passed test with duchy card in first position in deck\n"RESET);
	}
	else{
		printf(RED "Failed test with duchy card in first position in deck\n"RESET);
	}
	
	G.deck[player][0] = default_card;
	G.deck[player][deckMax - 1] = duchy;
	score = scoreFor(player, &G);
	
	if(score == 3){
		printf(GREEN "Passed test with duchy card in last position in deck\n"RESET);
	}
	else{
		printf(RED "Failed test with duchy card in last position in deck\n"RESET);
	}
	
	G.deck[player][deckMax - 1] = default_card;
	
	//test province card detection in hand while also checking boundary cases
	G.hand[player][0] = province;
	score = scoreFor(player, &G);
	
	if(score == 6){
		printf(GREEN "Passed test with province card in first position in hand\n"RESET);
	}
	else{
		printf(RED "Failed test with province card in first position in hand\n"RESET);
	}
	
	G.hand[player][0] = default_card;
	G.hand[player][handMax - 1] = province;
	score = scoreFor(player, &G);
	
	if(score == 6){
		printf(GREEN "Passed test with province card in last position in hand\n"RESET);
	}
	else{
		printf(RED "Failed test with province card in last position in hand\n"RESET);
	}
	
	G.hand[player][handMax - 1] = default_card;

	//test province card detection in discard while also checking boundary cases
	G.discard[player][0] = province;
	score = scoreFor(player, &G);
	
	if(score == 6){
		printf(GREEN "Passed test with province card in first position in discard\n"RESET);
	}
	else{
		printf(RED "Failed test with province card in first position in discard\n"RESET);
	}
	
	G.discard[player][0] = default_card;
	G.discard[player][discardMax - 1] = province;
	score = scoreFor(player, &G);
	
	if(score == 6){
		printf(GREEN "Passed test with province card in last position in discard\n"RESET);
	}
	else{
		printf(RED "Failed test with province card in last position in discard\n"RESET);
	}
	
	G.discard[player][discardMax - 1] = default_card;

	//test province card detection in deck while also checking boundary cases
	G.deck[player][0] = province;
	score = scoreFor(player, &G);
	
	if(score == 6){
		printf(GREEN "Passed test with province card in first position in deck\n"RESET);
	}
	else{
		printf(RED "Failed test with province card in first position in deck\n"RESET);
	}
	
	G.deck[player][0] = default_card;
	G.deck[player][deckMax - 1] = province;
	score = scoreFor(player, &G);
	
	if(score == 6){
		printf(GREEN "Passed test with province card in last position in deck\n"RESET);
	}
	else{
		printf(RED "Failed test with province card in last position in deck\n"RESET);
	}
	
	G.deck[player][deckMax - 1] = default_card;
	
	//test great_hall card detection in hand while also checking boundary cases
	G.hand[player][0] = great_hall;
	score = scoreFor(player, &G);
	
	if(score == 1){
		printf(GREEN "Passed test with great_hall card in first position in hand\n"RESET);
	}
	else{
		printf(RED "Failed test with great_hall card in first position in hand\n"RESET);
	}
	
	G.hand[player][0] = default_card;
	G.hand[player][handMax - 1] = great_hall;
	score = scoreFor(player, &G);
	
	if(score == 1){
		printf(GREEN "Passed test with great_hall card in last position in hand\n"RESET);
	}
	else{
		printf(RED "Failed test with great_hall card in last position in hand\n"RESET);
	}
	
	G.hand[player][handMax - 1] = default_card;

	//test great_hall card detection in discard while also checking boundary cases
	G.discard[player][0] = great_hall;
	score = scoreFor(player, &G);
	
	if(score == 1){
		printf(GREEN "Passed test with great_hall card in first position in discard\n"RESET);
	}
	else{
		printf(RED "Failed test with great_hall card in first position in discard\n"RESET);
	}
	
	G.discard[player][0] = default_card;
	G.discard[player][discardMax - 1] = great_hall;
	score = scoreFor(player, &G);
	
	if(score == 1){
		printf(GREEN "Passed test with great_hall card in last position in discard\n"RESET);
	}
	else{
		printf(RED "Failed test with great_hall card in last position in discard\n"RESET);
	}
	
	G.discard[player][discardMax - 1] = default_card;

	//test great_hall card detection in deck while also checking boundary cases
	G.deck[player][0] = great_hall;
	score = scoreFor(player, &G);
	
	if(score == 1){
		printf(GREEN "Passed test with great_hall card in first position in deck\n"RESET);
	}
	else{
		printf(RED "Failed test with great_hall card in first position in deck\n"RESET);
	}
	
	G.deck[player][0] = default_card;
	G.deck[player][deckMax - 1] = great_hall;
	score = scoreFor(player, &G);
	
	if(score == 1){
		printf(GREEN "Passed test with great_hall card in last position in deck\n"RESET);
	}
	else{
		printf(RED "Failed test with great_hall card in last position in deck\n"RESET);
	}
	
	G.deck[player][deckMax - 1] = default_card;
	
	//test math
	G.hand[player][0] = curse;
	G.discard[player][0] = duchy;
	G.deck[player][0] = province;
	score = scoreFor(player, &G);
	
	if(score == 8){
		printf(GREEN "Passed basic math test\n"RESET);
	}
	else{
		printf(RED "Failed basic math test\n"RESET);
	}
	
	G.hand[player][0] = default_card;
	G.discard[player][0] = default_card;
	G.deck[player][0] = default_card;
	
	//test gardens card
	G.hand[player][0] = gardens;
	score = scoreFor(player, &G);
	int expected = handMax / 10;
	if(score == expected){
		printf(GREEN "Passed gardens card test\n"RESET);
	}
	else{
		printf(RED "Failed gardens card test. Expected: %d Actual score: %d\n"RESET, expected, score);
	}
	
	//test cumulative gardens test
	G.discard[player][0] = gardens;
	G.deck[player][0] = gardens;
	expected += discardMax / 10;
	expected += deckMax / 10;
	
	score = scoreFor(player, &G);
	
	if(score == expected){
		printf(GREEN "Passed cumulative gardens card test\n"RESET);
	}
	else{
		printf(RED "Failed cumulative gardens card test. Expected: %d Actual score: %d\n"RESET, expected, score);
	}
	
	printf(YELLOW"Finished testing scoreFor()\n\n"RESET);
}

void unittest4()
{
	struct gameState G, G2;
	memset(&G, 49, sizeof(struct gameState));
	
	int card = curse, player = 0, passed = 1, i, j, dc;
	
	//set gameState to known values
	for(i = 0; i < 25; i++, card++){
		G.hand[player][i] = card;
		G.playedCards[i] = -999;
	}
	
	G.playedCardCount = 0;
	G.handCount[player] = 25;
	
	//make copy of gameState to revert to after each test
	memcpy(&G2, &G, sizeof(struct gameState));
	
	printf (YELLOW"\nBeginning test of discardCard()\n"RESET);
	
	//test discarding entire hand starting at first position moving to last
	for(i = 0; i < 25; i++){
		dc = discardCard(i, player, &G2, 0);
		if(G2.hand[player][G2.handCount[player]] != -1){
			passed = 0;
			printf(RED "Failed discarding hand in order from front to back at hand position %d and hand count %d\n"RESET, i, G2.handCount[player]);
		}
	}
	
	if(passed){
		printf(GREEN "Passed test discarding hand in order from front to back!\n"RESET);
	}
	
	passed = 1;
	
	//reset G2
	memcpy(&G2, &G, sizeof(struct gameState));
	
	//test discarding entire hand starting at last position and moving to first
	for(i = 24; i >= 0; i--){
		dc = discardCard(i, player, &G2, 0);
		if(G2.hand[player][G2.handCount[player]] != -1){
			passed = 0;
			printf(RED "Failed discarding hand in order from front to back at hand position %d and hand count %d\n"RESET, i, G2.handCount[player]);
		}
	}
	
	if(passed){
		printf(GREEN "Passed test discarding hand in order from back to front!\n"RESET);
	}
	
	passed = 1;
	
	//reset G2
	memcpy(&G2, &G, sizeof(struct gameState));
	
	//test removing individual cards at each position
	for(i = 0; i < 25; i++){
		int tmpCard = G2.hand[player][i];
		int lastCard = G2.hand[player][G2.handCount[player] - 1];
		dc = discardCard(i, player, &G2, 0);
		int newCard = G2.hand[player][i];
		if(newCard != lastCard){
			passed = 0;
			printf(RED "Failed discarding individual card at hand position %d and hand count %d\n"RESET, i, G2.handCount[player]);
		}
		
		G2.handCount[player] += 1;
		G2.hand[player][i] = tmpCard;
	}
	
	if(passed){
		printf(GREEN "Passed test discarding individual card at each position in hand!\n"RESET);
	}
	
	passed = 1;
	
	//reset G2
	memcpy(&G2, &G, sizeof(struct gameState));
	
	//test cards getting moved to the playedCards pile
	j = 0;
	
	for(i = 24; i >= 0; i--, j++){
		int tmpCard = G2.hand[player][i];
		dc = discardCard(i, player, &G2, 0);
		if(G2.playedCards[j] != tmpCard){
			passed = 0;
			printf(RED "Failed moving card from hand to played cards pile at hand position %d and hand count %d\n"RESET, i, G2.handCount[player]);
		}
	}
	
	if(passed){
		printf(GREEN "Passed test moving cards from hand to played cards pile!\n"RESET);
	}
	
	passed = 1;
	
	//reset G2
	memcpy(&G2, &G, sizeof(struct gameState));
	
	//test trash flag
	j = 0;
	
	for(i = 24; i >= 0; i--, j++){
		int tmpCard = G2.hand[player][i];
		dc = discardCard(i, player, &G2, 1);
		if(G2.playedCards[j] == tmpCard){
			passed = 0;
			printf(RED "Failed moving card to trash pile at hand position %d and hand count %d\n"RESET, i, G2.handCount[player]);
		}
	}
	
	if(passed){
		printf(GREEN "Passed test moving cards to trash pile!\n"RESET);
	}
	if(dc){} //clear warning for unused variable
	
	printf(YELLOW"Finished testing discardCard()\n\n"RESET);
}

int main()
{
	srand(time(NULL));
	adventurerTests();
	smithyTests();
	council_roomTests();
	sea_hagTest();
	unittest1();
	unittest2();
	unittest3();
	unittest4();
	return 0;
}
