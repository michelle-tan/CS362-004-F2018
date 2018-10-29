#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dominion.h"
#include "interface.h"
#include "rngs.h"

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
  int i,j;
  for(i = curse ; i < treasure_map + 1; i++){
    printf("\t%d x %d -> %d embargoTokens\n", state->supplyCount[i], i, state->embargoTokens[i]);
  }
  printf("It is %d's turn.\n", state->whoseTurn);
  for(i = 0; i < maxplayers; i++){
    printf("Player %d has...\n", i);
    printf("\t%d in hand\n", state->handCount[i]);
    for(j = 0 ; j < state->handCount[i]; j++){
      printf("\t\tCard %d: %d\n", j, state->hand[i][j]);
    }
    printf("\t%d in deck\n", state->deckCount[i]);
    for(j = 0 ; j < state->deckCount[i]; j++){
      printf("\t\tCard %d: %d\n", j, state->deck[i][j]);
    }
    printf("\t%d in discard\n", state->discardCount[i]);
    for(j = 0 ; j < state->discardCount[i]; j++){
      printf("\t\tCard %d: %d\n", j, state->discard[i][j]);
    }
  }
  printf("outpostPlayed: %d\n", state->outpostPlayed);
  printf("phase: %d\n",state->phase);
  printf("numActions: %d\n", state->numActions);
  printf("numBuys: %d\n", state->numBuys);
  printf("playedCardCount: %d\n", state->playedCardCount);
  printf("coins: %d\n", state->coins);
}

void constructGameStates(struct gameState* state, int numPlayers, int currentPlayer, int handCount, int deckCount, int handType, int deckType, int discardType, int numActions, int numBuys){

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
/*
//  int fillCard[] = {curse, estate, gardens, copper, gold, adventurer};
  int fillCard[] = {copper};
  int fillCardLength = 1;
  static int fillCardIdx_1 = 0;
  static int fillCardIdx_2 = 0;
  static int fillCardIdx_3 = 0;

 // int numPlayers[] = {2, MAX_PLAYERS};
  int numPlayers[] = {2};
  int numPlayersLength = 1;
  static int numPlayersIdx = 0;

//  int handCount[] = {0, 1, 5, MAX_HAND};
  int handCount[] = {1};
  static int handCountLength = 1;
  int handCountIdx = 0;

  int deckCount[] = {0, 1, 5, MAX_DECK};
  int deckCountLength = 1;
  static int deckCountIdx = 0;

  static int currentPlayerIdx = 0;

  state->numBuys = 1;
  state->numActions = 1;

// for each number of players
  for(numPlayersIdx; numPlayersIdx < numPlayersLength; numPlayersIdx++){

    state->numPlayers = numPlayers[numPlayersIdx];

// and each possible currentPlayer
    for(currentPlayerIdx; currentPlayerIdx < state->numPlayers; currentPlayerIdx++){

      state->whoseTurn = currentPlayerIdx;

// for each type of hand
      for(fillCardIdx_1; fillCardIdx_1 < fillCardLength; fillCardIdx_1++){

        memset(state->hand, fillCard[fillCardIdx_1], MAX_PLAYERS*MAX_HAND*sizeof(int));
// construct each handsize
        for(handCountIdx; handCountIdx < handCountLength; handCountIdx++){

          memset(state->handCount, handCount[handCountIdx], MAX_PLAYERS*sizeof(int));

// for each type of deck
          for(fillCardIdx_2; fillCardIdx_2 < fillCardLength; fillCardIdx_2++){

            memset(state->deck, fillCard[fillCardIdx_2], MAX_DECK*MAX_PLAYERS*sizeof(int));

// construct each decksize
            for(deckCountIdx; deckCountIdx < deckCountLength; deckCountIdx++){

              memset(state->deckCount, deckCount[deckCountIdx], MAX_PLAYERS * sizeof(int));

// finally, construct the discard while leaving room for cards...
printf("%d\n", MAX_DECK - deckCount[deckCountIdx] - handCount[handCountIdx]); 
             memset(state->discardCount,
				0,
                MAX_PLAYERS * sizeof(int));

              for(fillCardIdx_3; fillCardIdx_3 < fillCardLength; fillCardIdx_3++){
                memset(state->discard, fillCard[fillCardIdx_3], MAX_DECK*MAX_PLAYERS*sizeof(int));
				printState_(state);
                return 1;
              }
            }
          }
        }
      }
    }
  }
  return 0;
*/
}
