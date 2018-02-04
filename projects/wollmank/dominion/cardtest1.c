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
	int i, treasureCount = 0;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    int player = 0;
	struct gameState G, G2;
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
	
	printf (YELLOW"\nBeginning test of adventurer\n"RESET);
	
	//check if handCount increases by 2 and gains copper
	if(G2.handCount[player] == 7)
		printf(GREEN "Passed test: Adventurer accurately updates number of cards in hand!\n" RESET);
	else
		printf(RED "Failed test: Adventurer not accurately updating hand count. Expecting 7 : Actual = %d\n"RESET, G2.handCount[player]);
	
	for(i = 0; i < G2.handCount[player]; i++){
		if(G2.hand[player][i] == copper){
			treasureCount++;
		}
	}
	
	if(treasureCount == 2){
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
	
	return 0;
}
	
	
	