
#include "Game.h"

#include <iostream>

// Construct a Game to be played with the indicated players on a copy of the board b. The player on the south side always moves first.
Game::Game(const Board& b, Player* south, Player* north) : m_board(b), m_north(north), m_south(south), m_current(SOUTH) {}

// Display the game's board in a manner of your choosing, provided you show the names of the players and a reasonable representation of
//  the state of the board.
void Game::display() const
{
    int holes = m_board.holes();
    // print borders
    std::cout << "-------------------------------------" << std::endl << std::endl;

    // print north player
    for (int i = 0; i < holes / 2; i++)
    {
        std::cout << " ";
    }
    std::cout << m_north->name() << " (NORTH)" << std::endl;

    // print north side
    for (int i = 1; i <= holes; i++)
    {
        std::cout << "  " << m_board.beans(NORTH, i);
    }
    std::cout << std::endl;

    // print north pot, a space, south pot
    std::cout << m_board.beans(NORTH, POT);
    for (int i = 0; i < holes; i++)
    {
        std::cout << "   ";
    }
    std::cout << m_board.beans(SOUTH, POT) << std::endl;

    // print south side
    for (int i = 1; i <= holes; i++)
    {
        std::cout << "  " << m_board.beans(SOUTH, i);
    }
    std::cout << std::endl;

    // print south player
    for (int i = 0; i < holes / 2; i++)
    {
        std::cout << " ";
    }
    std::cout << m_south->name() << " (SOUTH)" << std::endl << std::endl;

    // print borders
    std::cout << "-------------------------------------" << std::endl << std::endl;

    // should look something like this:
    //---------------------------------
    //        Faust (NORTH)
    //        3  3  3  3
    //      0            0
    //        3  3  3  3
    //        John (SOUTH)
    //---------------------------------
}

// If the game is over (i.e., the move member function has been called and returned false), set over to true; otherwise, set over to
// false and do not change anything else. If the game is over, set hasWinner to true if the game has a winner, or false if it resulted
// in a tie. If hasWinner is set to false, leave winner unchanged; otherwise, set it to the winning side.
void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    if (m_board.beansInPlay(NORTH) == 0 && m_board.beansInPlay(SOUTH) == 0) // game is over
    {
        over = true;
        if (m_board.beans(NORTH, POT) == m_board.beans(SOUTH, POT)) // resulted in tie
        {
            hasWinner = false;
        }
        else // someone won
        {
            hasWinner = true;
            if (m_board.beans(NORTH, POT) > m_board.beans(SOUTH, POT))
            {
                winner = NORTH;
            }
            else // beans on SOUTH side are more than beans on NORTH side
            {
                winner = SOUTH;
            }
        }
    }
    else
    {
        over = false;
    }
}

// Attempt to make a complete move for the player playing side s. "Complete" means that the player sows the seeds from a hole and takes
// any additional turns required or completes a capture. Whenever the player gets an additional turn, you should display the board so
// someone looking at the screen can follow what's happening. If the move can be completed, return true; if not, because the move is not
// yet completed but side s has no holes with beans to pick up and sow, sweep any beans in s's opponent's holes into that opponent's pot
// and return false.
bool Game::move(Side s)
{
    if (m_board.beansInPlay(NORTH) == 0 && m_board.beansInPlay(SOUTH) == 0) // no more playable beans on this side
    {
        return false; // game is over
    }

    Side endside = s;
    int endhole = POT;

    if (s == NORTH)
    {
        while (endside == NORTH && endhole == POT) // while the turn ends in the pot
        {
            if (m_board.beansInPlay(NORTH) == 0) // if north is supposed to move but cannot
            {
                // sweep SOUTH's beans into their pot and end game
                for (int i = 1; i <= m_board.holes(); i++)
                {
                    m_board.moveToPot(SOUTH, i, SOUTH);
                }
                display();
                return false;
            }

            // otherwise, north can move properly
            int moveChoice = m_north->chooseMove(m_board, NORTH); // player starts by choosing a move
            std::cout << m_north->name() << " chose hole " << moveChoice << ". " << std::endl << std::endl;
            m_board.sow(NORTH, moveChoice, endside, endhole);     // sows beans appropriately
            display();
            if (endside == NORTH && m_board.beans(endside, endhole) == 1 && m_board.beans(SOUTH, endhole) != 0) // if capture
            {
                // move all beans to north's pot
                m_board.moveToPot(NORTH, endhole, NORTH);
                m_board.moveToPot(SOUTH, endhole, NORTH);
                std::cout << "CAPTURE!" << std::endl << std::endl;
                display();
            }
        }
        m_current = SOUTH;
        return true;
    }
    else // s == SOUTH
    {
        while (endside == SOUTH && endhole == POT) // while the turn ends in the pot
        {
            if (m_board.beansInPlay(SOUTH) == 0) // if south is supposed to move but cannot
            {
                // sweep NORTH's beans into their pot and end game
                for (int i = 1; i <= m_board.holes(); i++)
                {
                    m_board.moveToPot(NORTH, i, NORTH);
                }
                display();
                return false;
            }

            // otherwise, south can move properly
            int moveChoice = m_south->chooseMove(m_board, SOUTH); // player starts by choosing a move
            std::cout << m_south->name() << " chose hole " << moveChoice << ". " << std::endl << std::endl;
            m_board.sow(SOUTH, moveChoice, endside, endhole);     // sows beans appropriately
            display();
            if (endside == SOUTH && m_board.beans(endside, endhole) == 1 && m_board.beans(NORTH, endhole) != 0) // if capture
            {
                // move all beans to south's pot
                m_board.moveToPot(SOUTH, endhole, SOUTH);
                m_board.moveToPot(NORTH, endhole, SOUTH);
                std::cout << "CAPTURE!" << std::endl << std::endl;
                display();
            }
        }
        m_current = NORTH;
        return true;
    }
}

// Play the game. Display the progress of the game in a manner of your choosing, provided that someone looking at the screen can follow
// what's happening. If neither player is interactive, then to keep the display from quickly scrolling through the whole game, it would
// be reasonable periodically to prompt the viewer to press ENTER to continue and not proceed until ENTER is pressed. (The ignore
// function for input streams is useful here.) Announce the winner at the end of the game. You can apportion to your liking the responsibility
// for displaying the board between this function and the move function.
void Game::play()
{
    std::cout << "Welcome to Kahla!" << std::endl
        << std::endl;

    display();

    bool over = false;
    bool hasWinner = false;
    Side winner;

    while (!over)
    {
        if (move(m_current) == false) // attempt a move
        {
            over = true;
            status(over, hasWinner, winner); // check status of game
        }
        if (!m_north->isInteractive() && !m_south->isInteractive()) {
            std::cin.clear();
            std::cout << "Press Enter to continue...";
            std::cin.ignore(10000, '\n');
            std::cout << std::endl;
        }
        status(over, hasWinner, winner); // check status of game
    }

    // once the game is over:
    std::cout << "GAME OVER!" << std::endl << std::endl
        << "Results: " << std::endl;
    std::cout << "(NORTH) " << m_north->name() << "'s total beans: " << m_board.beans(NORTH, POT) << std::endl;
    std::cout << "(SOUTH) " << m_south->name() << "'s total beans: " << m_board.beans(SOUTH, POT) << std::endl;

    if (hasWinner)
    {
        std::cout << std::endl << "The winner is ";
        if (winner == NORTH)
        {
            std::cout << m_north->name() << "!";
        }
        else // winner == SOUTH
        {
            std::cout << m_south->name() << "!";
        }
        std::cout << std::endl;
    }

    else // it's a tie
    {
        std::cout << "It's a tie!";
    }
    std::cout << std::endl;
}

// Return the number of beans in the indicated hole or pot of the game's board, or −1 if the hole number is invalid. This function exists
// so that we and you can more easily test your program.
int Game::beans(Side s, int hole) const
{
    return m_board.beans(s, hole);
}