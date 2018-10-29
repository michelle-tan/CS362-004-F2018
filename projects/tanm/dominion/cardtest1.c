
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dominion.h"
#include "interface.h"
#include "rngs.h"
#include "myTestUtilities.h"


void testVillage(struct gameState* state){
  struct gameState stateCopy;
  memcpy(&stateCopy, state, sizeof(struct gameState));
  int copy_currentHandCount = stateCopy.handCount[stateCopy.whoseTurn];
  int copy_currentDeckCount = stateCopy.deckCount[stateCopy.whoseTurn];
  int copy_currentDiscardCount = stateCopy.discardCount[stateCopy.whoseTurn];
  cardEffect(village, 0, 0, 0, state, copy_currentHandCount-1, NULL);
  int currentHandCount = state->handCount[state->whoseTurn];
  int currentDeckCount = state->deckCount[state->whoseTurn];


  if(state->numActions != stateCopy.numActions+2){
    printf("FAILED TEST:village did not increase numActions by 2.\n");
	printf(" \tnumActions before village: %d\n\tnumActions after village: %d\n", stateCopy.numActions, state->numActions);
  }

  if(state->numBuys != stateCopy.numBuys){
    printf("FAILED TEST:village changed numBuys\n");
	printf("\tnumBuys before village: %d\n\tnumBuys after village: %d\n", stateCopy.numBuys, state->numBuys);
  }

  if(state->whoseTurn != stateCopy.whoseTurn){
    printf("FAILED TEST:village changed whoseTurn\n");
	printf("\tcurrentPlayer before village: %d\n\tcurrentPlayer after village: %d\n", stateCopy.whoseTurn, state->whoseTurn);
  }

// did the handCount stay the same
  if(currentHandCount != copy_currentHandCount ){
    printf("FAILED TEST:hand count did not stay the same.\n");
	printf("\thandCount before village: %d\n\thandCount after village: %d\n", copy_currentHandCount , currentHandCount);
  }

// if there were not enough cards in the deck to draw,
  if(copy_currentDeckCount < 1){
// discard should have been moved to the deck
    if(currentDeckCount != copy_currentDiscardCount- 1){
		printf("FAILED TEST: deck not decremented after moving discard to deck\n");
    }
  }
  else{
// did the deckCount decrease by 1
    if(currentDeckCount != copy_currentDeckCount  - 1){
      printf("FAILED TEST: deck did not decrease by 1 - deck had sufficient cards.\n");
	  printf("\tdeckCount before village: %d\n\tdeckCount after village: %d\n", copy_currentDeckCount , currentDeckCount);
    }
  }
// did the most recent hand card come from the deck, or is it still the card being tested?
    if(state->hand[state->whoseTurn][copy_currentHandCount-1] == village){
      printf("FAILED TEST: last card in hand is still village\n");
    }

// did the card get put in the discard?
  if(state->discardCount[state->whoseTurn] != stateCopy.discardCount[stateCopy.whoseTurn]+1){
    printf("FAILED TEST:discard pile did not increment\n");
    printf("\tdiscardCount before: %d\n\tdiscardCount after: %d\n", stateCopy.discardCount[stateCopy.whoseTurn], state->discardCount[state->whoseTurn]);
  }

  if(state->discard[state->whoseTurn][state->discardCount[state->whoseTurn]] != village){
    printf("FAILED TEST: top of discard pile is not village\n");
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
  int numPlayersLength = 2;
  int numPlayersIdx = 0;

  int handCount[] = {1, 5, MAX_HAND};
  int handCountLength = 3;
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
					      state.handCount[state.whoseTurn] -= 1;
					    }
						if(handCount < 0){printf("handCountIdx: %d\n", handCountIdx);}						
						if(deckCount < 0){printf("deckCountIdx: %d\n", deckCountIdx);}						
    					state.hand[state.whoseTurn][state.handCount[state.whoseTurn]-1] = village;
					    testVillage(&state);
						
					}
				}
			}
		}
	}
  }

}
