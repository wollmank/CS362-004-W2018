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
	
	return 0;
}
	
	