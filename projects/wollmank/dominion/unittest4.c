/**********************************************************
* Kyle Wollman
* CS362-400-W2018
* Assignment 3 - Unit testing
***********************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define YELLOW "\x1b[33m"

int main(){
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
	
	return 0;
}
		