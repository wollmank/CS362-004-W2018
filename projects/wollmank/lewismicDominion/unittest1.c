/**********************************************************
* Kyle Wollman
* CS362-400-W2018
* Assignment 3 - Unit testing
***********************************************************/

#include "dominion.h"
#include <string.h>
#include <stdio.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define YELLOW "\x1b[33m"

int main() {
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
	
	return 0;
}