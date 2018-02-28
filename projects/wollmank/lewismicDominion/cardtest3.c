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
	int passed = 1;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 3;
    int player = 0;
	struct gameState G, G2;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	//make a copy for testing
	memcpy(&G2, &G, sizeof(struct gameState));
	
	cardEffect(council_room, choice1, choice2, choice3, &G2, handpos, &bonus);
	
	printf (YELLOW"\nBeginning test of council room\n"RESET);
	
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
	
	return 0;
}