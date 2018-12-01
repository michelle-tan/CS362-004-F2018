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

void testSmithy(struct gameState* state, int handPos){
  static int testNum = 1;

  struct gameState stateCopy;
  memcpy(&stateCopy, state, sizeof(struct gameState));

  // create aliases for values in the copied gamestate
  int copy_currentHandCount = stateCopy.handCount[stateCopy.whoseTurn];
  int copy_currentDeckCount = stateCopy.deckCount[stateCopy.whoseTurn];
  int copy_currentDiscardCount = stateCopy.discardCount[stateCopy.whoseTurn];

  int expectedDiscardCount;

  cardEffect(smithy, 0, 0, 0, state, handPos, NULL);

// more aliases
  int currentHandCount = state->handCount[state->whoseTurn];
  int currentDeckCount = state->deckCount[state->whoseTurn];
  int currentDiscardCount = state->discardCount[state->whoseTurn];


printf("\n\n########## TEST NUMBER %d ##############\n", testNum);

  // smithy does not change numActions
  if(state->numActions != stateCopy.numActions){
    printf("FAILED TEST:\n");
    printf("\t numActions before smithy: %d\n", stateCopy.numActions);
    printf("\t numActions after smithy: %d\n", state->numActions);
  }

// smithy does not change numBuys
  if(state->numBuys != stateCopy.numBuys){
    printf("FAILED TEST:\n");
    printf("\t numBuys before smithy: %d\n", stateCopy.numBuys);
    printf("\t numBuys after smithy: %d\n", state->numBuys);
  }

// smithy does not change whoseTurn
  if(state->whoseTurn != stateCopy.whoseTurn){
    printf("FAILED TEST:\n");
    printf("\t whoseTurn before smithy: %d\n", stateCopy.whoseTurn);
    printf("\t whoseTurn after smithy: %d\n", state->whoseTurn);
  }

// did the handCount increase by three minus 1
  if(currentHandCount != copy_currentHandCount + 3-1){
    printf("FAILED TEST:\n");
    printf("\t smithy should add net 2 cards to the hand\n");
    printf("\t handCount before smithy: %d\n", copy_currentHandCount);
    printf("\t handCount after smithy: %d\n", currentHandCount);
   }

// if there were not enough cards in the deck to draw,
  if(copy_currentDeckCount < 3){

      expectedDiscardCount = 1;

// if there were less than 3 cards total, there should be nothing left in either deck/discard
    if(copy_currentDiscardCount < 3){
      if(currentDeckCount != 0){
        printf("FAILED TEST:\n");
        printf("\t fewer than 3 total cards available in deck+discard and resulting deckCount != 0\n");
        printf("\t actual deckCount: %d\n", currentDeckCount);
      }
    }

// if there were less than three cards in deck, discard should have been moved to the deck
    else if(currentDeckCount != copy_currentDiscardCount- 3 + copy_currentDeckCount){
		  printf("FAILED TEST:\n");
      printf("\t moved discard to deck but resulted in wrong deckCount\n");
      printf("\t deck before: %d, discard before: %d\n", copy_currentDeckCount, copy_currentDiscardCount);
      printf("\t deck after: %d, discard after: %d\n", currentDeckCount, currentDiscardCount);
 	 }
  }

// if there were enough cards in the deck; did the deckCount decrease by 3?
  else{

    expectedDiscardCount = copy_currentDiscardCount + 1;

    if(currentDeckCount != copy_currentDeckCount - 3){
      printf("FAILED TEST\n");
      printf("\t a deck with sufficient cards should decrease by 3\n");
	    printf("\t deckCount before smithy: %d\n", copy_currentDeckCount);
      printf("\t tdeckCount after smithy: %d\n", currentDeckCount);
    }
  }

// did the most recent hand card come from the deck, or is it still the card being tested?
  if(state->hand[state->whoseTurn][handPos] == smithy){
    printf("FAILED TEST: card in hand is still smithy\n");
  }

// did the card get put in the discard?
  if(currentDiscardCount != expectedDiscardCount){
    printf("FAILED TEST:\n");
    printf("\t discard pile did not increment\n");
    printf("\t discardCount expected: %d\n", expectedDiscardCount);
    printf("\t discardCount actual: %d\n", currentDiscardCount);
  }

  if(state->discard[state->whoseTurn][state->discardCount[state->whoseTurn]] != smithy){
    printf("FAILED TEST: top of discard pile is not smithy\n");
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
	if(state.handCount[state.whoseTurn] > MAX_HAND-3){
		state.handCount[state.whoseTurn] -= 3;
	}	
    handPos = rand() % state.numPlayers; // handPos must be less than numPlayers or else assignment 2's bug causes segfault
    state.hand[state.whoseTurn][handPos] = smithy;
    testSmithy(&state, handPos);
	numIterations--;
  }
}
