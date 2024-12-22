#include "Player.h"

#include <iostream>

// Abstract base class Player

Player::Player(std::string name) : m_name(name) {} // Create a Player with the indicated name.

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
int HumanPlayer::chooseMove(const Board& b, Side s) const
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
        std::cout << std::endl;

        if (b.beans(s, moveChoice) == -1 || moveChoice == 0) // invalid hole choice (out of range or pot)
        {
            std::cout << "The hole number must be from 1 to " << b.holes() << "." << std::endl << std::endl;
        }
        else if (b.beans(s, moveChoice) <= 0) // no beans in the hole
        {
            std::cout << "There are no beans in that hole!" << std::endl << std::endl;
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
int BadPlayer::chooseMove(const Board& b, Side s) const
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
    return moveChoice;
}

BadPlayer::~BadPlayer() {}

// Smart Player
SmartPlayer::SmartPlayer(std::string name) : Player(name) {}

int SmartPlayer::chooseMove(const Board& b, Side s) const
{
    JumpyTimer timer(1000);
    double timeLimit = MAX_TIME;

    int bestHole, value, depth(0); // initializing variables to track results
    chooseMoveHelper(b, s, bestHole, value, depth, timeLimit, timer); // calls helper function

    std::cerr << "Time passed: " << timer.actualElapsed() << " ms" << std::endl;
    return bestHole; // best move for depth
}

void SmartPlayer::chooseMoveHelper(const Board& b, Side s, int& bestHole, int& value, int currDepth, double limit, JumpyTimer& timer) const
{
    if (b.beansInPlay(s) == 0 || currDepth >= max_depth || limit <= 0) // if no move for player exists/game is over or reaches max depth/we run out of time
    {
        bestHole = -1;
        value = evaluate(b);
        return;
    }

    int beginning = 1; // index of first hole so we know root (we know the first move to be made)

    for (int h = 1; h <= b.holes(); h++) // for each hole
    {
        // start time for current branch 
        double startTime = timer.elapsed();

        // switching to breadth first vs depth first by splitting time
        double branchLimit = limit / (b.holes() - h + 1);

        if (b.beans(s, h) == 0) { //invalid hole, so we don't test moves
            beginning++; // haven't reached a valid hole yet, so go to next hole
        }
        else // start looking for best hole
        {
            Side opponent = (s == NORTH) ? SOUTH : NORTH;

            // initializing tracking variables
            Side endside;
            int endhole;
            int newValue, newHole, newhole2;

            Board newBoard(b); // copies board so we can "undo" sowing

            if (turnFinished(s, h, newBoard, endside, endhole)) { // for the case where it is now opponents turn
                chooseMoveHelper(newBoard, opponent, newHole, newValue, currDepth + 1, branchLimit, timer);
            }
            else { // still the same player's turn
                chooseMoveHelper(newBoard, s, newhole2, newValue, currDepth, branchLimit, timer); // note it's the same depth of move b/c we are still in the same turn
            }

            if (h == beginning) { // if the hole is the root, we need set initial values
                value = newValue;
                bestHole = h;
            }

            // see if new value is better
            if ((s == NORTH && newValue > value) || (s == SOUTH && newValue < value))
            {
                bestHole = h;
                value = newValue;
            }
        }   
        // Adjust remaining time
        limit -= (timer.elapsed() - startTime);
        if (limit <= 0)
            limit = 0;
    }  
}

bool SmartPlayer::turnFinished(Side s, int hole, Board& b, Side& endSide, int& endHole) const {

    Side opponent = (s == NORTH) ? SOUTH : NORTH;
    
    b.sow(s, hole, endSide, endHole);
    
    if (endHole == POT) { // take another turn because we ended in the pot
        return false;
    }
    else if (endSide == s) { // ended in a hole on player's side
        if (b.beans(s, endHole) == 1 && b.beans(opponent, endHole) > 0) // see if we can capture
        { 
            b.moveToPot(opponent, endHole, s);
            b.moveToPot(s, endHole, s);
        }
        return true; // as long as we don't end in the pot, it means our turn is finished 
    }
    else // we ended on the other side, which means the turn is finished
    {
        return true; 
    }
}

int SmartPlayer::evaluate(const Board& b) const
{
    // -9999 = best for south
    // negative = good for south
    // 9999 = best for north
    // positive = good for north
    // 0 = tie

    int beanDiff = b.beans(NORTH, POT) - b.beans(SOUTH, POT);
    int totalBeans = b.totalBeans();
    
    if (beanDiff == 0) // tie
    {
        return 0;
    }
    if (abs(beanDiff) > totalBeans / 2) // either of the sides will surely win
    {
        if (beanDiff > 0) // favors north
            return 9999;
        else // favors south
            return -9999;
    }
    return beanDiff;
}

SmartPlayer::~SmartPlayer() {}