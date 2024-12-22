#include "Board.h"
#include "Player.h"
#include "Game.h"

#include <iostream>
#include <cassert>

using namespace std;

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
}

void badAndHumanPlayerTests()
{
    HumanPlayer john("John");
    HumanPlayer faust("Faust");
    BadPlayer noob("Noob");

    Player *player1 = &john;
    Player *player2 = &faust;
    Player *player3 = &noob;

    assert(player1->name() == "John");
    assert(player2->name() == "Faust");
    assert(player3->name() == "Noob");

    assert(faust.isInteractive() && john.isInteractive());
    assert(noob.isInteractive() == false);

    Board b(3, 4);

    // testing BadPlayer
    assert(noob.chooseMove(b, NORTH) == 1);
    b.setBeans(NORTH, 2, 5);
    assert(noob.chooseMove(b, NORTH) == 2);
    b.setBeans(NORTH, 3, 5);
    assert(noob.chooseMove(b, NORTH) == 2);
    b.setBeans(NORTH, 3, 7);
    assert(noob.chooseMove(b, NORTH) == 3);
    b.setBeans(SOUTH, 1, 0);
    b.setBeans(SOUTH, 2, 0);
    b.setBeans(SOUTH, 3, 0);
    assert(noob.chooseMove(b, SOUTH) == -1);
    b.setBeans(SOUTH, 3, 3);

    // testing HumanPlayer
    for (int i = 0; i < 2; i++)
        john.chooseMove(b, SOUTH);
    for (int i = 0; i < 2; i++)
        john.chooseMove(b, NORTH);

    
}

void playTest()
{
    HumanPlayer john("John");
    BadPlayer noob("Noob");
    Board b2(4, 3);
    Game game1(b2, &john, &noob);
    game1.play();
}

int main()
{
    // boardTests();
    // badAndHumanPlayerTests();
    playTest();

    cout << "ALL TESTS PASSED";
}