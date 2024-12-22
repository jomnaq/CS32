#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Board.h"
#include "Side.h"

class Player
{
public:
    // Create a Player with the indicated name.
    Player(std::string name);

    // Return the name of the player.
    std::string name() const;

    // Return false if the player is a computer player. Return true if the player is human.
    // Most kinds of players will be computer players.
    virtual bool isInteractive() const;

    // Every concrete class derived from this class must implement this function so that if
    // the player were to be playing side s and had to make a move given board b, the function
    // returns the move the player would choose. If no move is possible, return −1.
    virtual int chooseMove(const Board& b, Side s) const = 0;

    // Since this class is designed as a base class, it should have a virtual destructor.
    virtual ~Player();

private:
    std::string m_name;
};

class HumanPlayer : public Player
{
public:
    HumanPlayer(std::string name);
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
    virtual ~HumanPlayer();
};

class BadPlayer : public Player
{
public:
    BadPlayer(std::string name);
    virtual int chooseMove(const Board& b, Side s) const;
    virtual ~BadPlayer();
};

class SmartPlayer : public Player
{
public:
    SmartPlayer(std::string name);
    virtual int chooseMove(const Board& b, Side s) const;
    virtual ~SmartPlayer();

private:
    // Heuristics:
    // difference of number of beans in pot 
        // north - south > 0, good for north
        // north - south < 0, good for south
    int evaluate(const Board &b) const; // given a board, returns a number measuring how good that position is for the player
    bool turnFinished(Side s, int hole, Board& b, Side& endSide, int& endHole) const; // returns true if the turn is over, returns false if the player takes another turn
    void chooseMoveHelper(const Board& b, Side s, int &bestHole, int &value, int currDepth, double limit, JumpyTimer &timer) const; // overloads chooseMove to help with recursion
    int max_depth = 10; // how far we want to look ahead. Now that there's a time limit, we can set it to bigger
};

#endif