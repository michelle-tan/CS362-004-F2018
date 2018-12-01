#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dominion.h"
#include "interface.h"
#include "rngs.h"
#include "myTestUtilities.h"


void testUpdateCoins(int numCoppers, int numSilvers, int numGolds, int bonus){
  int player = 0;
  struct gameState state;
  state.handCount[player] = numCoppers + numSilvers + numGolds;

  int expectedCoinage = numCoppers*1 + numSilvers*2 + numGolds*3 + bonus;

  int i;
  for(i = 0; i < numCoppers; i++){
    state.hand[player][i] = copper;
  }
  for(i ; i < numCoppers + numSilvers; i++){
    state.hand[player][i] = silver;
  }
  for(i ; i < numCoppers + numSilvers + numGolds; i++){
    state.hand[player][i] = gold;
  }

  updateCoins(player, &state, bonus);

  if(state.coins != expectedCoinage){
    printf("FAILED TEST: expected coinage -> %d \t actual coinage-> %d\n", expectedCoinage, state.coins);
    printf("\t%d coppers, %d silvers, %d golds, %d bonus\n", numCoppers, numSilvers, numGolds, bonus);
  }
}

int main(){

  int bonus;

  for(bonus = 0; bonus < 2; bonus++){
    testUpdateCoins(0,0,0,bonus);
    testUpdateCoins(1,0,0,bonus);
    testUpdateCoins(0,1,0,bonus);
    testUpdateCoins(0,0,1,bonus);
    testUpdateCoins(1,1,1,bonus);
    testUpdateCoins(MAX_HAND,0,0,bonus);
    testUpdateCoins(0,MAX_HAND,0,bonus);
    testUpdateCoins(0,0,MAX_HAND,bonus);
  }



}
