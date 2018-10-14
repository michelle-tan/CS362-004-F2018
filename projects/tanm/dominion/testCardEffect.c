#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <assert.h>

int main(int argc, char* argv[]){

// card to test
  int card = atoi(argv[1]);
  printf("Testing card : %d\n", card);

// this too is hardcoded. We aren't testing the whoseTurn() function.
  int currentPlayer = 0;
  int nextPlayer = 1;


// parameters are hardcoded for testing.
  int choice1 = 1; // these are for the mine
  int choice2 = silver;

  struct gameState statevar;
  struct gameState* state = &statevar;

  int handPos = 0;
  int* bonus;

// hardcoding the gamestate.

  state->numPlayers = 2; //number of players
  state->supplyCount[silver] = 1;  //only need one silver in the supply to test mine
//  state->embargoTokens[treasure_map+1];
//  state->outpostPlayed;
//  state->outpostTurn;
//  state->whoseTurn;
//  state->phase;
  state->numActions = 0; /* Starts at 1 each turn, it is 0 by the time cardEffect is called */
//  state->coins = 0; /* Use as you see fit! */
  state->numBuys = 1; /* Starts at 1 each turn */

// two cards in hand: card to test and a copper (for mine). none for the opponent
  state->hand[currentPlayer][handPos] = card;
  state->hand[currentPlayer][choice1] = copper;

  state->handCount[currentPlayer] = 2;
  state->handCount[nextPlayer] = 0;

// current gets 4 cards to draw
  state->deckCount[currentPlayer] = 4;
// next gets 1 card to draw (in case of council room)
  state->deckCount[nextPlayer] = 1;

// deck is full of coppers
  memset(state->deck[currentPlayer], copper, state->deckCount[currentPlayer]);
  memset(state->deck[nextPlayer], copper, state->deckCount[nextPlayer]);

// nothing in the discard piles
  state->discardCount[currentPlayer] =0;
  state->discardCount[nextPlayer] =0;

// discard pile is full of coppers, if not empty
  memset(state->discard[currentPlayer], copper, state->discardCount[currentPlayer]);
  memset(state->discard[nextPlayer], copper, state->discardCount[nextPlayer]);
 
   memset(state->playedCards, 0, MAX_DECK);
  state->playedCardCount = 0;

// these are declared in cardEffect. Copied and pasted to prevent anybody from complaining
//  int i;
 // int j;
  //int k;
//  int x;
//  int index;

  int tributeRevealedCards[2] = {-1, -1};
  int temphand[MAX_HAND];// moved above the if statement

  int  returnValue =5;

  switch(card){
    case adventurer:
	printf("Case adeventurer\n");
      returnValue = play_adventurer(state, currentPlayer);
	  break;

    case council_room:
   	  printf("Case council_room\n");
      returnValue = play_council_room(state, currentPlayer, handPos);
	  assert(state->numBuys == 2); // should go from 1 to 2
      assert(state->handCount[currentPlayer]==5); // from 2 to 1 to 5
      assert(state->handCount[nextPlayer]==1); // from 0 to 1
      assert(state->deckCount[currentPlayer] == 0); // from 4 to 0
	  assert(state->deckCount[nextPlayer] == 0); // from 1 to 0
	  break;

    case mine:
	printf("Case mine\n");
      returnValue = play_mine(state, currentPlayer, choice1, choice2, handPos);
      assert(state->handCount[currentPlayer]==1); // from 2 to 1 to 0 to 1
      assert(state->deckCount[currentPlayer]==4); // unchanged
	  break;

    case smithy:
	printf("Case smithy\n");
      returnValue = play_smithy(state, currentPlayer, handPos);
	  break;

    case village:
	printf("Case village\n");
      returnValue = play_village(state, currentPlayer, handPos);
	  assert(state->numActions == 2);
      assert(state->handCount[currentPlayer] == 2); // from 2 to 1 to 2
      assert(state->deckCount[currentPlayer] == 3); // from 4 to 3
	  break;

    default:
      printf("card not recognized\n");
  }

  printf("returnValue : %d\n", returnValue);

  return 0;
}
