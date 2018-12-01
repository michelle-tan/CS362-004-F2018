#include "dominion.h"

void printKingdomCards_(int* kingdomCards);
void printState_(struct gameState* state);
void constructGameStates(struct gameState* state, int numPlayers, int currentPlayer, int handCount, int deckCount, int handType, int deckType, int discardType, int numActions, int numBuys);
