#include "Player.h"

#include <iostream>

// Abstract base class Player

Player::Player(std::string name) : m_name (name) {} // Create a Player with the indicated name.

std::string Player::name() const // Return the name of the player.
{
    return m_name;
}

bool Player::isInteractive() const // Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
{
    return false;
}

Player::~Player() {} // Since this class is designed as a base class, it should have a virtual destructor.

// Human Player
HumanPlayer::HumanPlayer(std::string name) : Player(name) {}

bool HumanPlayer::isInteractive() const
{
    return true;
}

// chooses its move by prompting a person running the program for a move (reprompting if necessary until the person enters a valid hole number),
// and returning that choice. We will never test for a situation where the user doesn't enter an integer when prompted for a hole number.
int HumanPlayer::chooseMove(const Board &b, Side s) const
{
    if (b.beansInPlay(s) == 0) // side cannot make any plays with no beans
    {
        return -1;
    }

    int moveChoice = 0; // holds what hole they want to move upon

    while (true)
    {
        std::cout << Player::name() << "'s turn. Select a hole: ";
        std::cin >> moveChoice;

        if (b.beans(s, moveChoice) == -1 || moveChoice == 0) // invalid hole choice (out of range or pot)
        {
            std::cout << "The hole number must be from 1 to " << b.holes() << "." << std::endl;
        }
        else if (b.beans(s, moveChoice) <= 0) // no beans in the hole
        {
            std::cout << "There are no beans in that hole!" << std::endl;
        }
        else // valid move choice
        {
            break;
        }
    }
    return moveChoice;
}

HumanPlayer::~HumanPlayer() {}

// Bad Player
BadPlayer::BadPlayer(std::string name) : Player(name) {}

// A BadPlayer is a computer player that chooses an arbitrary valid move and returns that choice. "Arbitrary" can be what you like: leftmost,
// nearest to pot, fewest beans, random, etc.. The point of this class is to have an easy-to-implement class that at least plays legally.
int BadPlayer::chooseMove(const Board &b, Side s) const
{
    if (b.beansInPlay(s) == 0) // no possible moves
    {
        return -1;
    }

    int moveChoice = 1;

    for (int i = 2; i <= b.holes(); i++)
    {
        if (b.beans(s, i) > b.beans(s, moveChoice)) // picks the hole with the most beans
        {
            moveChoice = i;
        }
    }

    std::cout << Player::name() << " chose hole " << moveChoice << std::endl;
    return moveChoice;
}

BadPlayer::~BadPlayer() {}

// Smart Player
SmartPlayer::SmartPlayer(std::string name) : Player(name) {}

int SmartPlayer::chooseMove(const Board &b, Side s) const
{
    // to be completed...
    return -999999;
}

SmartPlayer::~SmartPlayer(){}