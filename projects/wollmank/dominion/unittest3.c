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


int main(){
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
	
	return 0;
} 
