#include "Board.h"
#include "Player.h"
#include "Game.h"
#include "Side.h"

#include <iostream>
#include <cassert>

using namespace std;

// for testing proper workings
void boardTests();
void playerTests();
void gameTests();

// for testing proper game play
void BadHumanPlayerTest();
void BadSmartPlayerTest();
void SmartHumanPlayerTest();

void TwoHumanPlayers();
void TwoSmartPlayers();
void TwoBadPlayers();

int main()
{
    // basic testing
    // boardTests();
    // playerTests();
    // gameTests();

    // for testing proper game play
    // BadHumanPlayerTest();
    // BadSmartPlayerTest();
    // SmartHumanPlayerTest();

    // TwoHumanPlayers();
    // TwoSmartPlayers();
    // TwoBadPlayers();
}

// timer implementations:
Timer::Timer()
{
    start();
}
void Timer::start()
{
    m_time = std::chrono::high_resolution_clock::now();
}
double Timer::elapsed() const
{
    std::chrono::duration<double, std::milli> diff =
        std::chrono::high_resolution_clock::now() - m_time;
    return diff.count();
}

// jumpy timer
JumpyTimer::JumpyTimer(int jumpInterval)
	: m_jumpInterval(jumpInterval), m_callsMade(0)
{
	actualElapsed();
}
double JumpyTimer::elapsed()
{
	m_callsMade++;
	if (m_callsMade == m_jumpInterval)
	{
		m_lastElapsed = m_timer.elapsed();
		m_callsMade = 0;
	}
	return m_lastElapsed;
}
double JumpyTimer::actualElapsed()
{
	m_lastElapsed = m_timer.elapsed();
	return m_lastElapsed;
}

// test function implementations
void boardTests()
{
    // Smallberg's board tests:
    Board b(3, 2);

    assert(b.holes() == 3);
    assert(b.totalBeans() == 12);
    assert(b.beans(SOUTH, POT) == 0);
    assert(b.beansInPlay(SOUTH) == 6);

    b.setBeans(SOUTH, 1, 1);
    b.moveToPot(SOUTH, 2, SOUTH);

    assert(b.totalBeans() == 11);
    assert(b.beans(SOUTH, 1) == 1);
    assert(b.beans(SOUTH, 2) == 0);
    assert(b.beans(SOUTH, POT) == 2);
    assert(b.beansInPlay(SOUTH) == 3);

    Side es;
    int eh;
    b.sow(SOUTH, 3, es, eh);
    assert(es == NORTH && eh == 3);
    assert(b.beans(SOUTH, 3) == 0 && b.beans(NORTH, 3) == 3);
    assert(b.beans(SOUTH, POT) == 3);
    assert(b.beansInPlay(SOUTH) == 1 && b.beansInPlay(NORTH) == 7);

    // Self - written tests:

    Board c(4, 2); // creating a new board

    // testing getter functions
    assert(c.totalBeans() == 16);
    assert(c.beansInPlay(NORTH) == 8);
    assert(c.beansInPlay(SOUTH) == 8);
    assert(c.beans(NORTH, 1) == 2);

    // testing setter functions
    assert(c.setBeans(NORTH, 3, 0));
    assert(c.setBeans(NORTH, 12, 4) == false);
    assert(c.setBeans(SOUTH, 3, -23) == false);

    // making sure beans changed after being set
    assert(c.totalBeans() == 16 - 2);
    assert(c.beansInPlay(NORTH) == 8 - 2);
    assert(c.beans(NORTH, 3) == 0);

    // testing sow function
    Side endSide = NORTH;
    int endHole = -999;
    assert(c.sow(NORTH, 3, endSide, endHole) == false);
    assert(endHole == -999 && endSide == Side::NORTH);
    assert(c.sow(SOUTH, 4, endSide, endHole));
    assert(endHole == 4 && endSide == NORTH);

    // testing moveToPot
    assert(c.moveToPot(NORTH, 1, NORTH));
    assert(c.beans(NORTH, POT) == 2);
    assert(c.beans(NORTH, 1) == 0);

    cout << "board tests passed" << endl;
}
void playerTests()
{
    // creating players
    HumanPlayer john("John");
    SmartPlayer faust("Faust");
    BadPlayer noob("Noob");

    Player* player1 = &john;
    Player* player2 = &faust;
    Player* player3 = &noob;

    // checking name()
    assert(player1->name() == "John");
    assert(player2->name() == "Faust");
    assert(player3->name() == "Noob");

    // checking interactivity
    assert(john.isInteractive());
    assert(noob.isInteractive() == false && faust.isInteractive() == false);

    Board b(3, 4);

    // testing BadPlayer
    assert(noob.chooseMove(b, NORTH) == 1); // chooses hole with largest num of beans
    b.setBeans(NORTH, 2, 5); // change hole to have more beans
    assert(noob.chooseMove(b, NORTH) == 2); // checking it picks the hole with more beans   
    // checking it picks the sooner hole if there are equal values
    b.setBeans(NORTH, 3, 5); 
    assert(noob.chooseMove(b, NORTH) == 2);
    b.setBeans(NORTH, 3, 7);
    assert(noob.chooseMove(b, NORTH) == 3);
    // checking that if it cannot pick any of the holes anymore that it doesn't do invalid moves
    b.setBeans(SOUTH, 1, 0);
    b.setBeans(SOUTH, 2, 0);
    b.setBeans(SOUTH, 3, 0);
    assert(noob.chooseMove(b, SOUTH) == -1);

    // testing HumanPlayer
    b.setBeans(SOUTH, 1, 3);
    assert(john.chooseMove(b, SOUTH)); 
    // For input testing for the human player:
    //  selecting 2 or 3 should result in a display that there are no more beans
    //  selecting 4 or 0 should result in a display that the hole is out of bounds
    //  selecting 1 should work properly.

    // Smallberg's tests:
    HumanPlayer hp("Marge");
    assert(hp.name() == "Marge" && hp.isInteractive());
    BadPlayer bp("Homer");
    assert(bp.name() == "Homer" && !bp.isInteractive());
    SmartPlayer sp("Lisa");
    assert(sp.name() == "Lisa" && !sp.isInteractive());
    Board c(3, 2);
    c.setBeans(SOUTH, 2, 0);
    cout << "=========" << endl;
    int n = hp.chooseMove(c, SOUTH);
    cout << "=========" << endl;
    assert(n == 1 || n == 3);
    n = bp.chooseMove(c, SOUTH);
    assert(n == 1 || n == 3);
    n = sp.chooseMove(c, SOUTH);
    assert(n == 1 || n == 3);

    cout << "player tests passed" << endl;
}
void gameTests() 
{
    // Smallberg's tests
    BadPlayer bp1("Bart");
    BadPlayer bp2("Homer");
    Board b(3, 0);
    b.setBeans(SOUTH, 1, 2);
    b.setBeans(NORTH, 2, 1);
    b.setBeans(NORTH, 3, 2);
    Game g(b, &bp1, &bp2);
    bool over;
    bool hasWinner;
    Side winner;
    //    Homer
    //   0  1  2
    // 0         0
    //   2  0  0
    //    Bart
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 0 &&
        g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 2 &&
        g.beans(SOUTH, 1) == 2 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);

    g.move(SOUTH);
    //   0  1  0
    // 0         3
    //   0  1  0
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
        g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 0 &&
        g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

    g.move(NORTH);
    //   1  0  0
    // 0         3
    //   0  1  0
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
        g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
        g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

    g.move(SOUTH);
    //   1  0  0
    // 0         3
    //   0  0  1
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
        g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
        g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 1);

    g.move(NORTH);
    //   0  0  0
    // 1         4
    //   0  0  0
    g.status(over, hasWinner, winner);
    assert(over && g.beans(NORTH, POT) == 1 && g.beans(SOUTH, POT) == 4 &&
        g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
        g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
    assert(hasWinner && winner == SOUTH);

    cout << "game tests passed" << endl;
}

void BadHumanPlayerTest()
{
    HumanPlayer john("John");
    BadPlayer noob("Noob");
    Board b2(4, 3);
    Game game1(b2, &john, &noob);
    game1.play();
}
void BadSmartPlayerTest()
{
    SmartPlayer smartie("Bot");
    BadPlayer noob("Noob");

    Board b(6, 4);
    Game g(b, &noob, &smartie);

    g.play();
}
void SmartHumanPlayerTest()
{
    SmartPlayer smartie("Smart Bot");
    HumanPlayer player("Player");

    Board b(3, 4);
    Game g(b, &smartie, &player);

    g.play();
}

void TwoHumanPlayers()
{
    HumanPlayer player1("P1");
    HumanPlayer player2("P2");

    Board b(3, 4);
    Game g(b, &player1, &player2);

    g.play();
}
void TwoSmartPlayers()
{
    SmartPlayer smartie("Smart Bot");
    SmartPlayer player("Radiant");

    Board b(3, 4);
    Game g(b, &smartie, &player);

    g.play();
}
void TwoBadPlayers()
{
    BadPlayer noob1("Iron");
    BadPlayer noob2("Noob");

    Board b(3, 4);
    Game g(b, &noob1, &noob2);

    g.play();
}