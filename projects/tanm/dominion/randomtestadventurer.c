
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

void testAdventurer(struct gameState* state, int handPos){
  static int testNum = 1;

  struct gameState stateCopy;
  memcpy(&stateCopy, state, sizeof(struct gameState));

// create aliases for values in the copied gamestate
  int copy_currentHandCount = stateCopy.handCount[stateCopy.whoseTurn];
  int copy_currentDeckCount = stateCopy.deckCount[stateCopy.whoseTurn];
  int copy_currentDiscardCount = stateCopy.discardCount[stateCopy.whoseTurn];

  printf("\n\n########## TEST NUMBER %d ##############\n", testNum);

// from statecopy, determine how many treasures should be drawn and from where (deck or discard)
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

// use adventurer
  cardEffect(adventurer, 0, 0, 0, state, handPos, NULL);

// more aliases
  int currentHandCount = state->handCount[state->whoseTurn];
  int currentDeckCount = state->deckCount[state->whoseTurn];
  int currentDiscardCount = state->discardCount[state->whoseTurn];

// adventurer does not change numActions
  if(state->numActions != stateCopy.numActions){
    printf("FAILED TEST:\n");
    printf("\t numActions before adventurer: %d\n", stateCopy.numActions);
    printf("\t numActions after adventurer: %d\n", state->numActions);
  }

// adventurer does not change numBuys
  if(state->numBuys != stateCopy.numBuys){
    printf("FAILED TEST:\n");
    printf("\t numBuys before adventurer: %d\n", stateCopy.numBuys);
    printf("\t numBuys after adventurer: %d\n", state->numBuys);
  }

// adventurer does not change whoseTurn
  if(state->whoseTurn != stateCopy.whoseTurn){
    printf("FAILED TEST:\n");
    printf("\t whoseTurn before adventurer: %d\n", stateCopy.whoseTurn);
    printf("\t whoseTurn after adventurer: %d\n", state->whoseTurn);
  }

// did the handCount get increased by 0, 1, or 2 treasures?
  if(deckTreasure+discardTreasure >= 2){ // if the treasures exist
    if(currentHandCount != copy_currentHandCount+2-1){
      printf("FAILED TEST:\n");
      printf("\t hand should have gained net 1 cards\n");
      printf("\t actual net gain to handCount: %d\n", currentHandCount - copy_currentHandCount);
    }
  }
  else{
    if(currentHandCount != copy_currentHandCount+deckTreasure+discardTreasure-1){
      printf("FAILED TEST:\n");
      printf("\t handCount should have gained 0 or 1 cards and subtracted 1\n");
	    printf("\t handCount before: %d\n", copy_currentHandCount);
      printf("\t handCount after: %d\n", currentHandCount);
      printf("\t expected gain: %d\n\tactual gain: %d\n", deckTreasure+discardTreasure-1, currentHandCount - copy_currentHandCount);
    }
  }

// if there are enough treasures to draw
  if(deckTreasure+discardTreasure >= 2){


// if both cards are drawn from the deck
    if(deckTreasure>=2){
      if(currentDeckCount != copy_currentDeckCount- 2){
        printf("FAILED TEST:\n");
		    printf("\t deck has 2 treasures but deck is not decremented by 2\n");
		    printf("\t deckCount before: %d\n", copy_currentDeckCount);
        printf("\t deckCount after: %d\n", currentDeckCount);
      }

      if(currentDiscardCount != copy_currentDiscardCount +1){
        printf("FAILED TEST:\n");
        printf("\t deck has 2 treasures, discard not incremented by 1\n");
        printf("\t discardCount before: %d\n", copy_currentDiscardCount);
        printf("\t discardCount after: %d\n", currentDiscardCount);
      }

    }

// if one treasure in deck and the rest are in discard
    else if(discardTreasure >= 1 && deckTreasure == 1){
      if(currentDeckCount != copy_currentDiscardCount- 1){
        printf("FAILED TEST:\n");
		    printf("\t 1 treasure in deck, 1 in discard; the discard that becomes the deck is not decremented by exactly 1\n");
      }

    // TODO: come up with a way to check if discardCount is as expected here?

    }

// if both treasures come from discard
    else{
      if(currentDeckCount != copy_currentDiscardCount- 2){
        printf("FAILED TEST:\n");
		    printf("\t both treasures are in discard, but the discard that becomes the deck is not decremented by 2\n");
      }

      // TODO: come up with a way to check if discardCount is as expected here?

    }
  }

// if there are not enough treasures in either deck or discard (both deck and discard must be exhausted)
  else if(deckTreasure+discardTreasure <2){
// check if deckCount is expected
	   if(currentDeckCount != 0){
       printf("FAILED TEST:\n");
		   printf("\t 0 treasures exist, deckCount should be 0\n");
		   printf("\t actual deckCount: %d\n", currentDeckCount);
    }

// check if discardCount is expected
    if(currentDiscardCount != copy_currentDeckCount + copy_currentDiscardCount - deckTreasure - discardTreasure + 1){
      printf("FAILED TEST:\n");
      printf("\t insufficient treasures exist, discardCount should be discard+deck counts minus 0 or 1 treasures, plus 1 discarded adventurer\n");
      printf("\t expected discardCount: %d\n", copy_currentDeckCount + copy_currentDiscardCount - deckTreasure - discardTreasure + 1);
      printf("\t actual discardCount: %d\n", currentDiscardCount);

    }
  }
/*
// if there is exactly one treasure
  else{

// if it is in the discard
      if(discardTreasure){
		    if(currentDeckCount != copy_currentDiscardCount - 1){
          printf("FAILED TEST:\n");
		      printf("\t 1 treasure exists in the discard, the deck should be the discard contents minus 1\n");
			    printf("\t actual deckCount: %d\n", currentDeckCount);
        }
      }

// if it is in the deck
    if(deckTreasure){
      if(currentDeckCount != 0){
        printf("FAILED TEST:\n");
			  printf("\t 1 treasure existed in the deck. Discard is moved to deck and is also exhausted.\n");
			  printf("\tactual deckCount: %d\n", currentDeckCount);
		  }
    }
  }

*/

// did the most recent hand card come from the deck, or is it still the card being tested?
  if(state->hand[state->whoseTurn][handPos] == adventurer){
    printf("FAILED TEST: card in hand is still adventurer\n");
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

  testNum++;
}

int main(){
  struct gameState state;
  srand(time(NULL));
  int numIterations = 10;
  int handPos;

  while(numIterations > 0){
    constructRandomGameState(&state);
// if there is no room to draw cards, then must remove two cards from hand
	if(state.handCount[state.whoseTurn] > MAX_HAND-2){
		state.handCount[state.whoseTurn] -= 2;
	}
    handPos = rand() % state.handCount[state.whoseTurn];
    state.hand[state.whoseTurn][handPos] = adventurer;
    testAdventurer(&state, handPos);
	numIterations--;
  }
}
