#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dominion.h"
#include "interface.h"
#include "rngs.h"
#include "myTestUtilities.h"

void testSmithy(struct gameState* state){
  struct gameState stateCopy;
  memcpy(&stateCopy, state, sizeof(struct gameState));
  int copy_currentHandCount = stateCopy.handCount[stateCopy.whoseTurn];
  int copy_currentDeckCount = stateCopy.deckCount[stateCopy.whoseTurn];
  int copy_currentDiscardCount = stateCopy.discardCount[stateCopy.whoseTurn];

  cardEffect(smithy, 0, 0, 0, state, copy_currentHandCount-1, NULL);

  int currentHandCount = state->handCount[state->whoseTurn];
  int currentDeckCount = state->deckCount[state->whoseTurn];

// smithy should not change number of actions
  if(state->numActions != stateCopy.numActions){
    printf("FAILED TEST:smithy should not change numActions\n");
	printf("\tnumActions before smithy: %d\n\tnumActions after smithy: %d\n", stateCopy.numActions, state->numActions);
  }

// smithy should not change number of buys
  if(state->numBuys != stateCopy.numBuys){
    printf("FAILED TEST:smithy should not change numBuys\n");
	printf("\tnumBuys before smithy: %d\n\tnumBuys after smithy: %d\n", stateCopy.numBuys, state->numBuys);
  }

// smithy shuld not change whose turn it is
  if(state->whoseTurn != stateCopy.whoseTurn){
    printf("FAILED TEST:smithy should not change whoseTurn\n");
	printf(" \tcurrentPlayer before smithy: %d\n\tcurrentPlayer after smithy: %d\n", stateCopy.whoseTurn, state->whoseTurn);
  }

// did the handCount increase by three minus 1
  if(currentHandCount != copy_currentHandCount + 3-1){
    printf("FAILED TEST:smithy should add net 2 cards to the hand\n");
	printf(" \thandCount before smithy: %d\n\thandCount after smithy: %d\n", copy_currentHandCount , currentHandCount);
  }
// if there were not enough cards in the deck to draw,
  if(copy_currentDeckCount < 3){
    if(copy_currentDiscardCount < 3){
      if(currentDeckCount != 0){
        printf("FAILED TEST: fewer than 3 total cards available and resulting deck count was not 0\n");
        printf("\tactual deckCount: %d\n", currentDeckCount);
      }
    }
// discard should have been moved to the deck
    else if(currentDeckCount != copy_currentDiscardCount- 3 + copy_currentDeckCount){
		printf("FAILED TEST: moved discard to deck but resulted in wrong deckCount\n");
        printf("\tdeck before: %d, discard before: %d\n\t deckCount after: %d\n", copy_currentDeckCount, copy_currentDiscardCount, currentDeckCount);
 	 }
  }
  else{
// there were enough cards in the deck; did the deckCount decrease by 3
    if(currentDeckCount != copy_currentDeckCount - 3){
      printf("FAILED TEST:a deck with sufficient cards should decrease by 3\n");
	  printf("\tdeckCount before smithy: %d\n\tdeckCount after smithy: %d\n", copy_currentDeckCount , currentDeckCount);
    }
  }

// did the most recent hand card come from the deck, or is it still the card being tested?
    if(state->hand[state->whoseTurn][copy_currentHandCount-1] == smithy){
      printf("FAILED TEST: card in hand is still smithy\n");
    }

// did the card get put in the discard?
  if(state->discardCount[state->whoseTurn] != stateCopy.discardCount[stateCopy.whoseTurn]+1){
    printf("FAILED TEST:discard pile did not increment\n");
    printf("\tdiscardCount before: %d\n\tdiscardCount after: %d\n", stateCopy.discardCount[stateCopy.whoseTurn], state->discardCount[state->whoseTurn]);
  }

  if(state->discard[state->whoseTurn][state->discardCount[state->whoseTurn]] != smithy){
    printf("FAILED TEST: top of discard pile is not smithy\n");
  }
}

int main(){
  struct gameState state;
  int fillCard[] = {gardens, copper, adventurer};
  int fillCardLength = 3;
  int fillCardIdx_1 = 0;
  int fillCardIdx_2 = 0;
  int fillCardIdx_3 = 0;

  int numPlayers[] = {2, MAX_PLAYERS};
  int numPlayersLength =2;
  int numPlayersIdx = 0;

  int handCount[] = {1, 5, MAX_HAND};
  int handCountLength = 1; // cannot use this array, as handPos cannot be greater than MAX_PLAYERS or else there will be a segfault with the bug introduced in HW2, making gcov useless?
  int handCountIdx = 0;

  int deckCount[] = {0, 5, MAX_DECK};
  int deckCountLength = 3;
  int deckCountIdx = 0;
 
  for(numPlayersIdx=0; numPlayersIdx < numPlayersLength; numPlayersIdx++){
	for(fillCardIdx_1=0; fillCardIdx_1 < fillCardLength; fillCardIdx_1++){
		for(handCountIdx=0; handCountIdx < handCountLength; handCountIdx++){
	  		for(fillCardIdx_2=0; fillCardIdx_2 < fillCardLength; fillCardIdx_2++){
				for(deckCountIdx=0; deckCountIdx < deckCountLength; deckCountIdx++){			 
					for(fillCardIdx_3=0; fillCardIdx_3 < fillCardLength; fillCardIdx_3++){
						constructGameStates(&state, numPlayers[numPlayersIdx], 0, handCount[handCountIdx], deckCount[deckCountIdx], fillCard[fillCardIdx_1], fillCard[fillCardIdx_2], fillCard[fillCardIdx_3],1 ,1);

					    if(handCount == MAX_HAND){
					      state.handCount[state.whoseTurn] -= 3;
					    }
    					state.hand[state.whoseTurn][state.handCount[state.whoseTurn]-1] = smithy;
					    testSmithy(&state);
						
					}
				}
			}
		}
	}
  }
}
