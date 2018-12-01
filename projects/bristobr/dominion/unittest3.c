#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dominion.h"
#include "interface.h"
#include "rngs.h"
#include "myTestUtilities.h"


void testGainCard(int supplyPos, int supplyCount, int toFlag, int expectedReturnValue, char* testName){
  int player = 0;
  struct gameState state;
  state.numPlayers = 2;
  state.deckCount[player] = 0;
  state.handCount[player] = 0;
  state.discardCount[player] = 0;
  state.supplyCount[supplyPos] = supplyCount;

  int returnValue;
  returnValue = gainCard(supplyPos, &state, toFlag, player);

  if(returnValue != expectedReturnValue){
    printf("FAILED TEST: %s WITH RETURN VALUE %d (EXPECTED %d)\n", testName, returnValue, expectedReturnValue);
	printState_(&state);
    return;
  }
// if gainCard succeeded
  if(returnValue == 0){
    if(state.supplyCount[supplyPos] != supplyCount-1){
      printf("FAILED TEST: %s. supplyCount not decremented.\n", testName);
   	  printState_(&state);
	  return;
    }
  
    if(toFlag == 1){
      if(state.deckCount[player] != 1 && state.deck[player][state.deckCount[state.whoseTurn]-1] != supplyPos){
        printf("FAILED TEST: %s. player deck not updated correctly.\n", testName);
	    printState_(&state);
      }
      return;
    }
    if(toFlag == 2){
   //   if(state.handCount[player] != 1 && state.hand[player][1] != supplyPos){
      if(state.handCount[player] != 1 && state.hand[player][state.handCount[state.whoseTurn]-1] != supplyPos){
        printf("FAILED TEST: %s. player hand not updated correctly.\n", testName);
    	printState_(&state);
      }
      return;
    }
    else{
   //   if(state.discardCount[player] != 1 && state.discard[player][1] != supplyPos){
      if(state.discardCount[player] != 1 && state.discard[player][state.discardCount[state.whoseTurn]-1] != supplyPos){
        printf("FAILED TEST: %s. player discard not updated correctly.\n", testName);
   	    printState_(&state);
      }
      return;
    }
  }
}

int main(){

  testGainCard(4, 1, 1, 0, "gain the one available copper to the deck");
  testGainCard(4, 1, 2, 0, "gain the one available copper to the hand");
  testGainCard(4, 1, 3, 0, "gain the one available copper to the discard");

  testGainCard(4, 0, 1, -1, "gain an unavailable copper to the deck");
  testGainCard(4, 0, 2, -1, "gain an unavailable copper to the hand");
  testGainCard(4, 0, 3, -1, "gain an unavailable copper to the discard");

}
