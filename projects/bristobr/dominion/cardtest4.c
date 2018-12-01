#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dominion.h"
#include "interface.h"
#include "rngs.h"
#include "myTestUtilities.h"

void testCouncil_Room(struct gameState* state){
  struct gameState stateCopy;
  memcpy(&stateCopy, state, sizeof(struct gameState));
  cardEffect(council_room, 0, 0, 0, state, stateCopy.handCount[stateCopy.whoseTurn]-1, NULL);

// council_room should not change number of actions
  if(state->numActions != stateCopy.numActions){
    printf("FAILED TEST:\n \tnumActions before council_room: %d\n\tnumActions after council_room: %d\n", stateCopy.numActions, state->numActions);
  }

// council_room should add one buy
  if(state->numBuys != stateCopy.numBuys + 1){
    printf("FAILED TEST:\n \tnumBuys before council_room: %d\n\tnumBuys after council_room: %d\n", stateCopy.numBuys, state->numBuys);
  }

// council_room shuld not change whose turn it is
  if(state->whoseTurn != stateCopy.whoseTurn){
    printf("FAILED TEST:\n \tcurrentPlayer before council_room: %d\n\tcurrentPlayer after council_room: %d\n", stateCopy.whoseTurn, state->whoseTurn);
  }
  int i, cardsToDraw;
  for(i=0; i<state->numPlayers; i++){
    if(i == state->whoseTurn){cardsToDraw=4-1;} // keeping in mind that the current player will discard the council room card
    else{cardsToDraw=1;}
// did the handCount increase by 4 (or 1, depending on who we ask)
    if(state->handCount[i] != stateCopy.handCount[i] +cardsToDraw){
      printf("FAILED TEST:\n \thandCount before council_room: %d\n\thandCount after council_room: %d\n", stateCopy.handCount[i] , state->handCount[i]);
    }

// if there were not enough cards in the deck to draw,
    if(stateCopy.deckCount[i] < cardsToDraw){
      if(stateCopy.discardCount[i] < cardsToDraw){
        if(state->deckCount[i] != 0){
          printf("FAILED TEST: fewer than 4 cards available and resulting deck count was not 0\n");
          printf("\tactual deckCount: %d\n", state->deckCount[i]);
        }
      }
// discard should have been moved to the deck
      else if(state->deckCount[i] != stateCopy.discardCount[i]- cardsToDraw + stateCopy.handCount[i]){
      // FAILED TEST: DECK IS NOT DECREMENTED AFTER MOVING DISCARD TO DECK
      }
    }
    else{
// there were enough cards in the deck; did the deckCount decrease by 4 (or 1)
      if(state->deckCount[i] != stateCopy.deckCount[i] - cardsToDraw){
        printf("FAILED TEST:\n \tdeckCount before council_room: %d\n\tdeckCount after council_room: %d\n", stateCopy.deckCount[i] , state->deckCount[i]);
      }
    }
  }
// did the most recent hand card come from the deck, or is it still the card being tested?
  if(state->hand[state->whoseTurn][stateCopy.handCount[i]-1] == council_room){
    printf("FAILED TEST: card in hand is still council_room\n");
  }

// did the card get put in the discard?
// not checking this for the other users. that seems like a lot
  if(stateCopy.deckCount[stateCopy.whoseTurn] < 4){
    if(state->discardCount[state->whoseTurn] != 1){
      printf("FAILED TEST: discard not 1 after moving discard to deck\n");
    }
  }
  else{
    if(state->discardCount[state->whoseTurn] != stateCopy.discardCount[stateCopy.whoseTurn]+1){
      printf("FAILED TEST:discard pile did not increment, deck was not depleted\n");
      printf("\tdiscardCount before: %d\n\tdiscardCount after: %d\n", stateCopy.discardCount[stateCopy.whoseTurn], state->discardCount[state->whoseTurn]);
    }
  }

  if(state->discard[state->whoseTurn][state->discardCount[state->whoseTurn]] != council_room){
    printf("FAILED TEST: top of discard pile is not council_room\n");
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
 
  int i;

  for(numPlayersIdx=0; numPlayersIdx < numPlayersLength; numPlayersIdx++){
	for(fillCardIdx_1=0; fillCardIdx_1 < fillCardLength; fillCardIdx_1++){
		for(handCountIdx=0; handCountIdx < handCountLength; handCountIdx++){
	  		for(fillCardIdx_2=0; fillCardIdx_2 < fillCardLength; fillCardIdx_2++){
				for(deckCountIdx=0; deckCountIdx < deckCountLength; deckCountIdx++){			 
					for(fillCardIdx_3=0; fillCardIdx_3 < fillCardLength; fillCardIdx_3++){
						constructGameStates(&state, numPlayers[numPlayersIdx], 0, handCount[handCountIdx], deckCount[deckCountIdx], fillCard[fillCardIdx_1], fillCard[fillCardIdx_2], fillCard[fillCardIdx_3],1 ,1);

					    if(handCount == MAX_HAND){
      						for(i=0; i < state.numPlayers; i++){
						        state.handCount[i]--; // give each player room to draw a card (if current play has max cards, so does everyone else.)
						      }
					        state.handCount[state.whoseTurn] -= 3; // give a hand that has a max number of cards room to draw more - otherwise a segfault
					        state.hand[state.whoseTurn][state.handCount[state.whoseTurn]-1] = council_room; // the last card in the hand is the one being played
					    }

					    testCouncil_Room(&state);
						
					}
				}
			}
		}
	}
  }

}
