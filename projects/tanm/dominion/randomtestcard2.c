
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "dominion.h"
#include "interface.h"
#include "rngs.h"
#include "myTestUtilities.h"


void testVillage(struct gameState* state, int handPos){
  static int testNum = 1;
  struct gameState stateCopy;
  memcpy(&stateCopy, state, sizeof(struct gameState));

  // create aliases for values in the copied gamestate
  int copy_currentHandCount = stateCopy.handCount[stateCopy.whoseTurn];
  int copy_currentDeckCount = stateCopy.deckCount[stateCopy.whoseTurn];
  int copy_currentDiscardCount = stateCopy.discardCount[stateCopy.whoseTurn];

  cardEffect(village, 0, 0, 0, state, handPos, NULL);

  // more aliases
    int currentHandCount = state->handCount[state->whoseTurn];
    int currentDeckCount = state->deckCount[state->whoseTurn];
    int currentDiscardCount = state->discardCount[state->whoseTurn];

  printf("\n\n########## TEST NUMBER %d ##############\n", testNum);
  // village adds 2 actions
  if(state->numActions != stateCopy.numActions + 2){
    printf("FAILED TEST:\n");
    printf("\t numActions before village: %d\n", stateCopy.numActions);
    printf("\t numActions after village: %d\n", state->numActions);
  }

// village does not change numBuys
  if(state->numBuys != stateCopy.numBuys){
    printf("FAILED TEST:\n");
    printf("\t numBuys before village: %d\n", stateCopy.numBuys);
    printf("\t numBuys after village: %d\n", state->numBuys);
  }

// village does not change whoseTurn
  if(state->whoseTurn != stateCopy.whoseTurn){
    printf("FAILED TEST:\n");
    printf("\t whoseTurn before village: %d\n", stateCopy.whoseTurn);
    printf("\t whoseTurn after village: %d\n", state->whoseTurn);
  }

// did the handCount stay the same
  if(currentHandCount != copy_currentHandCount ){
    printf("FAILED TEST:\n");
    printf("\t hand count did not stay the same (gain one, lose one).\n");
	  printf("\t handCount before village: %d\n", copy_currentHandCount);
    printf("\t handCount after village: %d\n", currentHandCount);
  }

// if there were not enough cards in the deck to draw,
  if(copy_currentDeckCount < 1){
// discard should have been moved to the deck
    if(currentDeckCount != copy_currentDiscardCount- 1){
		  printf("FAILED TEST: deck not decremented after moving discard to deck\n");
    }

    if(currentDiscardCount != 1){
      printf("FAILED TEST: discard does not contain 1 after moving discard to deck & discarding village");
    }
  }

// if there were enough cards in the deck to draw
  else{
// did the deckCount decrease by 1?
    if(currentDeckCount != copy_currentDeckCount  - 1){
      printf("FAILED TEST:\n");
      printf("\t deck did not decrease by 1 though deck had sufficient cards.\n");
	    printf("\t deckCount before village: %d\n", copy_currentDeckCount);
      printf("\t deckCount after village: %d\n", currentDeckCount);
    }

    if(currentDiscardCount != copy_currentDiscardCount +1){
      printf("FAILED TEST:\n");
      printf("\t discard did not increase by 1, deck had sufficient cards to draw.\n");
      printf("\t discardCount before village: %d\n", copy_currentDiscardCount);
      printf("\t discardCount after village: %d\n", currentDiscardCount);
    }
  }
// did the most recent hand card come from the deck, or is it still the card being tested?
    if(state->hand[state->whoseTurn][copy_currentHandCount-1] == village){
      printf("FAILED TEST: last card in hand is still village\n");
    }

  if(state->discard[state->whoseTurn][state->discardCount[state->whoseTurn]] != village){
    printf("FAILED TEST: top of discard pile is not village\n");
  }

  testNum++;
}


int main(){
  struct gameState state;
  srand(time(NULL));
  int numIterations = 10000;
  int handPos;

  while(numIterations > 0){
    constructRandomGameState(&state);
	if(state.handCount[state.whoseTurn] > MAX_HAND-1){
		state.handCount[state.whoseTurn] -= 1;
	}
	if(state.handCount[state.whoseTurn] == 0){
		state.handCount[state.whoseTurn] = 1;
		state.hand[state.whoseTurn][0] = village;
	}
	handPos = rand() % state.handCount[state.whoseTurn]; 
    state.hand[state.whoseTurn][handPos] = village;
    testVillage(&state, handPos);
	numIterations--;
  }
}
