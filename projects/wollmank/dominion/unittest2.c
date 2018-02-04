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
	int i, over;
	struct gameState G;
	memset(&G, 49, sizeof(struct gameState));

	//set supplyCount to known value
	for(i = 0; i < treasure_map + 1; i++){
		G.supplyCount[i] = MAX_DECK;
	}
	
	printf (YELLOW"\nBeginning test of isGameOver()\n"RESET);
	
	//check if game is over with full supply
	over = isGameOver(&G);
	if(over == 0){
		printf(GREEN "Passed test with fully supplied deck\n" RESET);
	}
	
	else{
		printf(RED "Failed test with fully supplied deck\n" RESET);
	}
	
	//check if game ends when province cards are empty
	G.supplyCount[province] = 0;
	
	over = isGameOver(&G);
	if(over == 1){
		printf(GREEN "Passed test when province is empty\n" RESET);
	}
	
	else{
		printf(RED "Failed test when province is empty\n" RESET);
	}
	
	//check if game ends when province is negative value
	G.supplyCount[province] = -1;
	
	over = isGameOver(&G);
	if(over == 1){
		printf(GREEN "Passed test when province is negative value\n" RESET);
	}
	
	else{
		printf(RED "Failed test when province is negative value\n" RESET);
	}
	
	//check if game ends when 1 supply pile runs out
	G.supplyCount[province] = MAX_DECK;
	G.supplyCount[smithy] = 0;
	
	over = isGameOver(&G);
	if(over == 0){
		printf(GREEN "Passed test when 1 supply pile runs out\n" RESET);
	}
	
	else{
		printf(RED "Failed test when 1 supply pile runs out\n" RESET);
	}
	
	//check if game ends when 2 supply piles run out
	G.supplyCount[smithy] = MAX_DECK;
	G.supplyCount[baron] = G.supplyCount[gardens] = 0;
	
	over = isGameOver(&G);
	if(over == 0){
		printf(GREEN "Passed test when 2 supply piles run out\n" RESET);
	}
	
	else{
		printf(RED "Failed test when 2 supply piles run out\n" RESET);
	}
	
	//check if game ends when 3 supply piles run out
	G.supplyCount[baron] = G.supplyCount[gardens] = MAX_DECK;
	G.supplyCount[gold] = G.supplyCount[adventurer] = G.supplyCount[minion] = 0;
	
	over = isGameOver(&G);
	if(over == 1){
		printf(GREEN "Passed test when 3 supply piles run out\n" RESET);
	}
	
	else{
		printf(RED "Failed test when 3 supply piles run out\n" RESET);
	}
	
	//check if game ends when supply piles become negative
	G.supplyCount[gold] = G.supplyCount[adventurer] = G.supplyCount[minion] = MAX_DECK;
	G.supplyCount[mine] = G.supplyCount[remodel] = G.supplyCount[duchy] = -1;
	
	over = isGameOver(&G);
	if(over == 1){
		printf(GREEN "Passed test when 3 supply piles become negative\n" RESET);
	}
	
	else{
		printf(RED "Failed test when 3 supply piles become negative\n" RESET);
	}
	
	//check boundary cases for supply piles
	G.supplyCount[gold] = G.supplyCount[adventurer] = G.supplyCount[minion] = MAX_DECK;
	G.supplyCount[curse] = G.supplyCount[treasure_map] = G.supplyCount[sea_hag] = 0;
	
	over = isGameOver(&G);
	if(over == 1){
		printf(GREEN "Passed test when first and last supply piles run out\n" RESET);
	}
	
	else{
		printf(RED "Failed test when first and last supply piles run out\n" RESET);
	}
	
	printf(YELLOW"Finished testing isGameOver()\n\n"RESET);
		
	return 0;
}

