CFLAGS= -Wall -g -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

#myTestUtilities: dominion.o rngs.o
#	gcc -o myTestUtilities myTestUtilities.c -g dominion.o rngs.o $(CFLAGS)

unittest1: dominion.o unittest1.c myTestUtilities.c rngs.o
	gcc -o unittest1 unittest1.c -g dominion.o rngs.o myTestUtilities.c $(CFLAGS)

unittest2: dominion.o unittest2.c myTestUtilities.o rngs.o
	gcc -o unittest2 unittest2.c -g dominion.o rngs.o myTestUtilities.c $(CFLAGS)

unittest3: dominion.o unittest3.c myTestUtilities.o rngs.o
	gcc -o unittest3 unittest3.c -g dominion.o rngs.o myTestUtilities.c $(CFLAGS)

unittest4: dominion.o unittest4.c myTestUtilities.o rngs.o
	gcc -o unittest4 unittest4.c -g dominion.o rngs.o myTestUtilities.c $(CFLAGS)

cardtest1: dominion.o cardtest1.c myTestUtilities.o rngs.o
	gcc -o cardtest1 cardtest1.c -g dominion.o rngs.o myTestUtilities.c $(CFLAGS)

cardtest2: dominion.o cardtest2.c myTestUtilities.o rngs.o
	gcc -o cardtest2 cardtest2.c -g dominion.o rngs.o myTestUtilities.c $(CFLAGS)

cardtest3: dominion.o cardtest3.c myTestUtilities.o rngs.o
	gcc -o cardtest3 cardtest3.c -g dominion.o rngs.o myTestUtilities.c $(CFLAGS)

cardtest4: dominion.o cardtest4.c myTestUtilities.o rngs.o
	gcc -o cardtest4 cardtest4.c -g dominion.o rngs.o myTestUtilities.c $(CFLAGS)

randomtestadventurer: dominion.o randomtestadventurer.c myTestUtilities.o rngs.o
	gcc -o randomtestadventurer randomtestadventurer.c -g dominion.o rngs.o myTestUtilities.c $(CFLAGS)

randomtestcard1: dominion.o randomtestcard1.c myTestUtilities.o rngs.o
	gcc -o randomtestcard1 randomtestcard1.c -g dominion.o rngs.o myTestUtilities.c $(CFLAGS)

randomtestcard2: dominion.o randomtestcard2.c myTestUtilities.o rngs.o
	gcc -o randomtestcard2 randomtestcard2.c -g dominion.o rngs.o myTestUtilities.c $(CFLAGS)

cardtest1.out: cardtest1 dominion.o
	./cardtest1 > cardtest1.out
	gcov dominion.c -bf >> cardtest1.out

cardtest2.out: cardtest2 dominion.o
	./cardtest2 > cardtest2.out
	gcov dominion.c -bf >> cardtest2.out

cardtest3.out: cardtest3 dominion.o
	./cardtest3 > cardtest3.out
	gcov dominion.c -bf >> cardtest3.out

cardtest4.out: cardtest4 dominion.o
	./cardtest4 > cardtest4.out
	gcov dominion.c -bf >> cardtest4.out

unittest1.out: unittest1 dominion.o
	./unittest1 > unittest1.out
	gcov dominion.c -bf >> unittest1.out

unittest2.out: unittest2 dominion.o
	./unittest2 > unittest2.out
	gcov dominion.c -bf >> unittest2.out

unittest3.out: unittest3 dominion.o
	./unittest3 > unittest3.out
	gcov dominion.c -bf >> unittest3.out

unittest4.out: unittest4 dominion.o
	./unittest4 > unittest4.out
	gcov dominion.c -bf >> unittest4.out

randomtestadventurer.out: randomtestadventurer dominion.o
	./randomtestadventurer > randomtestadventurer.out
	gcov dominion.c -bf >> randomtestadventurer.out

randomtestcard1.out: randomtestcard1 dominion.o
	./randomtestcard1 > randomtestcard1.out
	gcov dominion.c -bf >> randomtestcard1.out

randomtestcard2.out: randomtestcard2 dominion.o
	./randomtestcard2 > randomtestcard2.out
	gcov dominion.c -bf >> randomtestcard2.out

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

# make clean no longer removes .out files
clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so testDrawCard testDrawCard.exe
