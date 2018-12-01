#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dominion.h"
#include "interface.h"
#include "rngs.h"
#include <time.h>

void printKingdomCards_(int* kingdomCards){
  int i;
  for(i = 0; i < 10; i++){
    printf("Card %d: %d\n", i, kingdomCards[i]);
  }
	printf("fin");
}

void printState_(struct gameState* state){
  int maxplayers= state->numPlayers > MAX_PLAYERS ? MAX_PLAYERS : state->numPlayers;
  printf("Supply contains:\n");
  int i;
  for(i = curse ; i < treasure_map + 1; i++){
    printf("\t%d x %d -> %d embargoTokens\n", state->supplyCount[i], i, state->embargoTokens[i]);
  }
  printf("there are %d players\n", state->numPlayers);
  printf("It is %d's turn.\n", state->whoseTurn);
  for(i = 0; i < maxplayers; i++){
    printf("Player %d has...\n", i);
    printf("\t%d in hand\n", state->handCount[i]);
//    for(j = 0 ; j < state->handCount[i]; j++){
//      printf("\t\tCard %d: %d\n", j, state->hand[i][j]);
//    }
    printf("\t%d in deck\n", state->deckCount[i]);
//    for(j = 0 ; j < state->deckCount[i]; j++){
//      printf("\t\tCard %d: %d\n", j, state->deck[i][j]);
//    }
    printf("\t%d in discard\n", state->discardCount[i]);
//    for(j = 0 ; j < state->discardCount[i]; j++){
//      printf("\t\tCard %d: %d\n", j, state->discard[i][j]);
//    }
  }
  printf("outpostPlayed: %d\n", state->outpostPlayed);
  printf("phase: %d\n",state->phase);
  printf("numActions: %d\n", state->numActions);
  printf("numBuys: %d\n", state->numBuys);
  printf("playedCardCount: %d\n", state->playedCardCount);
  printf("coins: %d\n", state->coins);
}

// void shuffle(struct student* studentArr, int n)
// this function shuffles the array of students passed in
// ENTRY: studentArr is a pointer to an array of students and n is the size of that array
// EXIT: the array is shuffled randomly
void shuffleIntArray(int* arr, int n){

    int* indicesShuffled = malloc( n * sizeof(int));

    // initialize all the indices to 0 (not struck out yet)
    for(int i = 0; i < n; i++){
        indicesShuffled[i] = 0;
    }

    int swap;             // a temp variable for swapping during the shuffle
    int swapIndex;                   // the random number representing the index to swap arr[i] with


    // NOW SHUFFLE
    for(int i = (n-1); i > 1; i--){
        swapIndex = rand() % (i + 1); // generate a random number between 0 and i, inclusive

        // go through the subarray 0 -> randomNum: in order to know when you are at the randomNum(th) unstruck index, we
        // keep track of how many in the subarray have been already struck out. For each one, +1 to the destination index
        for(int j = 0; j <= swapIndex; j++){
            if(indicesShuffled[swapIndex] == 1){
                swapIndex++;
            }
        }

        // once we have the index of the element to swap with, we do the swap
        swap = arr[i];
        arr[i] = arr[swapIndex];
        arr[swapIndex] = swap;


        // and finally, set this index as having been struck out
        indicesShuffled[swapIndex] = 1;

    }
    free(indicesShuffled);
}


void constructRandomGameState(struct gameState* state){

  state->numActions = 1;
  state->numBuys = 1;
  state->phase = 0;
  state->playedCardCount = 0;
//  srand(time(NULL));
  state->numPlayers = rand() % (MAX_PLAYERS-1);
  state->numPlayers += 2;

  state->whoseTurn = rand() % state->numPlayers;


  int* supplyCards = malloc(27 * sizeof(int));

  int i, j;
  for(i=0; i <= treasure_map; i++){
    supplyCards[i] = i;
  }

  shuffleIntArray(supplyCards+7, 20);

  for(i=0; i< state->numPlayers; i++){

    state->handCount[i] = rand() % (MAX_HAND - 1);
    for(j=0; j < state->handCount[i]; j++){
      state->hand[i][j] = supplyCards[rand() % 17];
    }

    state->deckCount[i] = rand() % (MAX_DECK - 1);
    for(j=0; j < state->deckCount[i]; j++){
      state->deck[i][j] = supplyCards[rand() % 17];
    }

    state->discardCount[i] = rand() % (MAX_DECK - state->deckCount[i] + 1);
    for(j=0; j < state->discardCount[i]; j++){
      state->discard[i][j] = supplyCards[rand() % 17];
    }
  }

  if(state->numPlayers == 2){
    state->supplyCount[curse] = rand() % 11;
    state->supplyCount[estate] = rand() % 9;
    state->supplyCount[duchy] = rand() % 9;
    state->supplyCount[province] = rand() % 9;
  }
  else{
    if(state->numPlayers == 3){
      state->supplyCount[curse] = rand() % 21;
    }
    else{
      state->supplyCount[curse] = rand() % 31;
    }

    state->supplyCount[estate] = rand() % 13;
    state->supplyCount[duchy] = rand() % 13;
    state->supplyCount[province] = rand() % 13;
  }
  state->supplyCount[copper] = rand() % (60 - (7*state->numPlayers) +1);
  state->supplyCount[silver] = rand() % 41;
  state->supplyCount[gold] = rand() % 31;

  for(i = 7; i < 18; i++){
	if(supplyCards[i] == great_hall || supplyCards[i] == gardens){
		if(state->numPlayers == 2){
			state->supplyCount[supplyCards[i]] = rand() %  9;
		}
		else{
			state->supplyCount[supplyCards[i]] = rand() % 13;
		}
	}
	else{
		state->supplyCount[supplyCards[i]] = rand() % 11;
	}
  }
  free(supplyCards);
}

void constructGameStates(struct gameState* state, int numPlayers, int whoseTurn, int handCount, int deckCount, int handType, int deckType, int discardType, int numActions, int numBuys){
//printf("numPlayers = %d\n", numPlayers);
	state->numActions = numActions;
	state->numBuys = numBuys;
	state->numPlayers = numPlayers;
	state->playedCardCount = 0;
	int i, j;
	for(i = 0 ; i < numPlayers; i++){
		state->handCount[i] = handCount;
		for(j = 0; j < handCount; j++){
			state->hand[i][j] = handType;
		}
		state->deckCount[i] = deckCount;
		for(j = 0; j < deckCount; j++){
			state->deck[i][j] = deckType;
		}
		state->discardCount[i] = MAX_DECK - deckCount;

		for(j = 0; j < state->discardCount[i]; j++){
			state->discard[i][j] = discardType;
		}
	}
//printState_(state);

}
