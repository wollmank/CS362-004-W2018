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
	int i,n,p;
	int timesMOff = 0, totalMOff = 0, timesDOff = 0, totalDOff = 0, timesTOff = 0, totalTOff;
	int treasureTest = 1, movedTest = 1, discardTest = 1, shuffleTest = 1, shuffleCalled = 0;
	int temphand[MAX_DECK];
	int numTests = 20000;
	float avgMOff, avgDOff, avgTOff;
	
	struct gameState G, pre;

	//seed random number generator
	SelectStream(54);
	PutSeed(414);
	
	
	for(n = 0; n < numTests; n++)
	{
		//set game to random state
		for(i = 0; i < sizeof(struct gameState); i++)
		{
			((char*)&G)[i] = floor(Random() * 256);
		}
		
		p = floor(Random() * 2);
	
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
			adventurerFunc(0, p, 0, temphand, &G);
			
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
			adventurerFunc(0, p, 0, temphand, &G);
			
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
	
	return 0;
}