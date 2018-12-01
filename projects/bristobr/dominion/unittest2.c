#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dominion.h"
#include "interface.h"
#include "rngs.h"
#include "myTestUtilities.h"

void testIsGameOver(int seed, int emptyProvincePile, int numOtherEmptyPiles, int expectedReturnValue, char* testName){
  int returnValue;
  struct gameState state;

  int* kingdom = kingdomCards(7,8,9,10,11,12,13,14,15,16); // adventurer thru great_hall

  initializeGame(2, kingdom, seed, &state);

  if(emptyProvincePile){
    state.supplyCount[province] = 0;
  }
  else{
    state.supplyCount[province] = 8;
  }

  int i;
  for(i = 0; i < numOtherEmptyPiles; i++){
    state.supplyCount[i+7] = 0;
  }

  returnValue = isGameOver(&state);

  if(returnValue != expectedReturnValue){
    printf("FAILED TEST: %s WITH RETURN VALUE %d (EXPECTED: %d)\n", testName, returnValue, expectedReturnValue);
    printKingdomCards_(kingdom);
    printState_(&state);
  }

  free(kingdom);

}

int main() {
  int seed = 1;
// game should not be over - provinces not empty, no empty piles
  testIsGameOver(seed, 0, 0, 0, "provinces not empty, no empty supply piles.");

// game is over - provinces empty, all supply piles full
  testIsGameOver(seed, 1, 0, 1, "provinces empty, no empty supply piles.");

// game is not over - provinces not empty, 2 supply piles empty
    testIsGameOver(seed, 0, 2, 0, "provinces not empty, 2 empty supply piles.");

// game is over - provinces not empty, 3 supply piles empty
  testIsGameOver(seed, 0, 3, 1, "provinces not empty, 3 empty supply piles.");

// game is over - provinces not empty, 4 supply piles empty
  testIsGameOver(seed, 0, 4, 1, "provinces not empty, 3 empty supply piles.");

// game is over - provinces empty AND 3 supply piles empty
    testIsGameOver(seed, 1, 3, 1, "provinces empty, 3 empty supply piles.");

}
