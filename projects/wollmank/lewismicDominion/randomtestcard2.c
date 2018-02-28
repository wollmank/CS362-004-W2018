/**********************************************************
* Kyle Wollman
* CS362-400-W2018
* Assignment 4 - Random testing
***********************************************************/

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

int main()
{
	srand(time(NULL));
	int i,n,p, j;
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
		
		int handPos = floor(Random() * G.handCount[p]);
		
		G.numBuys = 1;
		
		//create a pre call copy of gameState
		memcpy(&pre, &G, sizeof(struct gameState));
		
		council_roomFunc(p, &G, handPos);
		
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
		
	return 0;
}
		