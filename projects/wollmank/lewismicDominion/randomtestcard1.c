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
	int i,n,p;
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
		
		smithyFunc(p, handPos, &G);
		
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
	
	return 0;
}
