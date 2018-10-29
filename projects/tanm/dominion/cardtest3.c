
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dominion.h"
#include "interface.h"
#include "rngs.h"
#include "myTestUtilities.h"

void testAdventurer(struct gameState* state){
  struct gameState stateCopy;
  memcpy(&stateCopy, state, sizeof(struct gameState));

  int copy_currentHandCount = stateCopy.handCount[stateCopy.whoseTurn];
  int copy_currentDeckCount = stateCopy.deckCount[stateCopy.whoseTurn];
  int copy_currentDiscardCount = stateCopy.discardCount[stateCopy.whoseTurn];

  // from statecopy, determine how many treasures will be drawn and from where
  int i, card;
  int deckTreasure=0;
  int discardTreasure=0;
  for(i=0; i< copy_currentDeckCount;i++){
    card = stateCopy.deck[stateCopy.whoseTurn][i];
    if(card== copper || card==silver || card==gold){
      deckTreasure++;
    }
  }
  for(i=0; i< stateCopy.discardCount[stateCopy.whoseTurn];i++){
    card = stateCopy.discard[stateCopy.whoseTurn][i];
    if(card== copper || card==silver || card==gold){
      discardTreasure++;
    }
  }
	printf("deckT: %d, discardT: %d\n", deckTreasure, discardTreasure);
  cardEffect(adventurer, 0, 0, 0, state, copy_currentHandCount-1, NULL);
  int currentHandCount = state->handCount[state->whoseTurn];
  int currentDeckCount = state->deckCount[state->whoseTurn];

// adventurer does not change numActions
  if(state->numActions != stateCopy.numActions){
    printf("FAILED TEST:\n \tnumActions before adventurer: %d\n\tnumActions after adventurer: %d\n", stateCopy.numActions, state->numActions);
  }

// adventurer does not change numBuys
  if(state->numBuys != stateCopy.numBuys){
    printf("FAILED TEST:\n \tnumBuys before adventurer: %d\n\tnumBuys after adventurer: %d\n", stateCopy.numBuys, state->numBuys);
  }

// adventurer does not change whoseTurn
  if(state->whoseTurn != stateCopy.whoseTurn){
    printf("FAILED TEST:\n \tcurrentPlayer before adventurer: %d\n\tcurrentPlayer after adventurer: %d\n", stateCopy.whoseTurn, state->whoseTurn);
  }

// did the handCount get increased by 0, 1, or 2 treasures?
  if(deckTreasure+discardTreasure >= 2){ // if the treasures exist
    if(currentHandCount != copy_currentHandCount+2-1){
      printf("FAILED TEST: hand should have gained net 1 cards\n");
      printf("\tactual gain: %d\n", currentHandCount - copy_currentHandCount);
    }
  }
  else{
    if(currentHandCount != copy_currentHandCount+deckTreasure+discardTreasure-1){
      printf("FAILED TEST: hand should have gained 0 or 1 cards and subtracted 1\n");
	printf("handcount before %d after %d\n", copy_currentHandCount, currentHandCount);  
     printf("\texpected gain: %d\n\tactual gain: %d\n", deckTreasure+discardTreasure-1, currentHandCount - copy_currentHandCount);
    }
  }

// if there were not enough cards in the deck to draw,
  if(deckTreasure+discardTreasure >= 2){
    if(deckTreasure>=2){
// both cards are drawn from the deck
      if(currentDeckCount != copy_currentDeckCount- 2){
		printf("FAILED TEST: deck has 2 treasures but deck is not decremented by 2\n");
		printf("\tdeckCount before: %d\n\tdeckCount after: %d\n", copy_currentDeckCount, currentDeckCount);
      }
    }
    else if(discardTreasure >= 1 && deckTreasure == 1){
      if(currentDeckCount != copy_currentDiscardCount- 1){
		printf("FAILED TEST: deck has 1 treasures but the discard that becomes the deck is not decremented by 1\n");
      }
    }
    else{ // both treasures come from discard
      if(currentDeckCount != copy_currentDiscardCount- 2){
		printf("FAILED TEST: deck has 0 treasures but the discard that becomes the deck is not decremented by 2\n");
      }
    }
  }
  else if(deckTreasure+discardTreasure == 0){
	if(currentDeckCount != 0){
		printf("FAILED TEST: 0 treasures exist, deckCount should be 0\n");
		printf("\tdeckCount: %d\n", currentDeckCount);
    }
  }
  else{
    if(discardTreasure){
		if(currentDeckCount != copy_currentDiscardCount - 1){
		  	printf("FAILED TEST: one treasure exists in the discard, the deck should be the discard contents minus 1\n");
			printf("\tactual deckCount: %d\n", currentDeckCount);
        }
	}
	if(deckTreasure){
		if(currentDeckCount != 0){
			printf("FAILED TEST: one treasure existed in the deck. Discard is moved to deck and is also exhausted.\n");
			printf("\tactual deckCount: %d\n", currentDeckCount);
		}
	}
  }


// did the most recent hand card come from the deck, or is it still the card being tested?
  if(state->hand[state->whoseTurn][copy_currentHandCount] == adventurer){
    printf("FAILED TEST: last card in hand is still adventurer\n");
  }

// did the card get put in the discard? gotta search all cards to know for sure
  int seenFlag=0;
  for(i=0; i < state->discardCount[state->whoseTurn]; i++){
    if(state->discard[state->whoseTurn][i] == adventurer){
      seenFlag=1;
      break;
    }
  }
  if(!seenFlag){
    printf("FAILED TEST: discard pile does not contain adventurer\n");
  }
}

int main(){
  struct gameState state;
  int fillCard[] = {gardens, copper, village};
  int fillCardLength = 3;
  int fillCardIdx_1 = 0;
  int fillCardIdx_2 = 0;
  int fillCardIdx_3 = 0;

  int numPlayers[] = {2, MAX_PLAYERS};
  int numPlayersLength =2;
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
					      state.handCount[state.whoseTurn] -= 2;
					    }
    					state.hand[state.whoseTurn][state.handCount[state.whoseTurn]-1] = adventurer;
					    testAdventurer(&state);
						
					}
				}
			}
		}
	}
  }
}
