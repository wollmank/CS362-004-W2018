/**********************************************************
* Kyle Wollman
* CS362-400-W2018
* Assignment 3 - Unit testing
***********************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define YELLOW "\x1b[33m"

int main(){
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    int player = 0;
	struct gameState G, G2;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	//make a copy for testing
	memcpy(&G2, &G, sizeof(struct gameState));
	
	cardEffect(smithy, choice1, choice2, choice3, &G2, handpos, &bonus);
	
	printf (YELLOW"\nBeginning test of smithy\n"RESET);
	
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
	
	return 0;
	
}