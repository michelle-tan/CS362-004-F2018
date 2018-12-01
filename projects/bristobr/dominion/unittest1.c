#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dominion.h"
#include "interface.h"
#include "rngs.h"
#include "myTestUtilities.h"

// examine gameState
int examineGameState(int numPlayers, struct gameState* state){
  if(numPlayers == 2){
    if(state->supplyCount[curse] != 10){
      return curse;
    }
    if(state->supplyCount[estate] != 8){
      return estate;
    }
    if(state->supplyCount[duchy] != 8){
      return duchy;
    }
    if(state->supplyCount[province] != 8){
      return province;
    }
  }
  else{
    if(numPlayers == 3){
      if(state->supplyCount[curse] != 20){
        return curse;
      }
	}
    else{
      if(state->supplyCount[curse] != 30){
        return curse;
      }
    }
  
    if(state->supplyCount[estate] != 12){
      return estate;
    }
    if(state->supplyCount[duchy] != 12){
      return duchy;
    }
    if(state->supplyCount[province] != 12){
      return province;
    }
  }
  if(state->supplyCount[copper] != 60 - (7* numPlayers)){
    return copper;
  }
  if(state->supplyCount[silver] != 40){
    return silver;
  }
  if(state->supplyCount[gold] != 30){
    return gold;
  }

// for each player, check that their decks have the appropriate number of cards
  int i, j;
  int numCoppers, numEstates;
  for(i = 0; i < numPlayers; i++){
    numCoppers = 0;
    numEstates = 0;

    if(i==state->whoseTurn){
      if(state->deckCount[i] != 5){
        return -3;
      }
      if(state->handCount[i] != 5){
        return -4;
      }
    }
	else{
      if(state->deckCount[i] != 10){return -3;}
      if(state->handCount[i] != 0){return -4;}
	}
    for(j = 0 ; j < state->handCount[i] ; j++){
      if(state->hand[i][j] == copper){
        numCoppers++;
        continue;
      }
      if(state->hand[i][j] == estate){
        numEstates++;
        continue;
      }
    }
	if(i==state->whoseTurn && state->coins!= numCoppers){
	  return -13;
	}
    for(j = 0 ; j < state->deckCount[i] ; j++){
      if(state->deck[i][j] == copper){
        numCoppers++;
        continue;
      }
      if(state->deck[i][j] == estate){
        numEstates++;
        continue;
      }
    }
    if(numCoppers != 7){return -5;}
    if(numEstates != 3){return -5;}
  }
  for (i = 0; i <= treasure_map; i++){
    if(state->embargoTokens[i] != 0){
      return -6;
    }
  }

  if(state->outpostPlayed != 0){
    return -7;
  }
  if(state->phase != 0){
    return -8;
  }
  if(state->numActions != 1){
    return -9;
  }
  if(state->numBuys != 1){
    return -10;
  }
  if(state->playedCardCount != 0){
    return -11;
  }
  if(state->whoseTurn != 0){
    return -12;
  }
  return -1;
}


// test initializeGame
void testInitializeGame(int seed, int numPlayers, int repetitiveSupply, int expectedReturnValue, char* testName){

  int* kingdom;
  struct gameState state;
  int returnValue;
  int failure = -1;  // -1 is a passing grade

  kingdom = kingdomCards(adventurer, baron, great_hall, feast, gardens, treasure_map, smithy, minion, sea_hag, mine);
 
  if(repetitiveSupply){
    kingdom = memset(kingdom, adventurer, 10*sizeof(int));
  }

  returnValue = initializeGame(numPlayers, kingdom, seed, &state);

  
  if(returnValue == -1 && expectedReturnValue == -1){
	free(kingdom);
	return;
  }

  if(returnValue != expectedReturnValue){
    failure = -2; // -2 means return value unexpected
  }

  if(returnValue == 0 && expectedReturnValue == 0){
// look at the gamestate with respect to number of players and see if things are as expected
    failure = examineGameState(numPlayers, &state);  // 0 and above corresponds to the card that didn't make sense.
  }

  if(failure != -1){
    printf("FAILED: %s WITH CODE %d\n", testName, failure);
    printf("Expected return value: %d\n", expectedReturnValue);
    printf("Actual return value: %d\n", returnValue);
    printf("numPlayers = %d\n", numPlayers);
    printKingdomCards_(kingdom);
    printState_(&state);
  }

  free(kingdom);
}

int main(){
  int seed = 1;
// more than MAX_PLAYERS players, no duplicates in kingdom cards, expect return -1
  testInitializeGame(seed, MAX_PLAYERS+1, 0, -1, "numPlayers > MAX_PLAYERS");

// fewer than MAX_PLAYERS players, no duplicates in kingdomCards, expect return 0
  testInitializeGame(seed, MAX_PLAYERS-1, 0, 0, "numPlayers < MAX_PLAYERS");

// exactly MAX_PLAYERS players, no duplicates in kingdomCards, expect return 0
  testInitializeGame(seed, MAX_PLAYERS, 0, 0, "numPlayers = MAX_PLAYERS");

// more than 2 players, no duplicates in kingdom cards, expect return 0
  testInitializeGame(seed, 3, 0, 0, "numPlayers > 2");

// fewer than 2 players, no duplicates in kingdomCards, expect return -1
  testInitializeGame(seed, 1, 0, -1, "numPlayers < 2");

// exactly 2 players, no duplicates in kingdomCards, expect return 0
  testInitializeGame(seed, 2, 0, 0, "numPlayers = 2");

// valid numPlayers, kingdomCards all adventurers, expect return -1
  testInitializeGame(seed, 2, 1, -1, "kingdomCards are all adventurers");

}
