
#include "Board.h"
#include "Side.h"

// Construct a Board with the indicated number of holes per side (not counting the pot)
// and initial number of beans per hole. If nHoles is not positive, act as if it were 1;
// if nInitialBeansPerHole is negative, act as if it were 0.
Board::Board(int nHoles, int nInitialBeansPerHole)
{
    // checking validity of inputs
    if (nHoles <= 0)
    {
        m_holes = 1;
    }
    else
    {
        m_holes = nHoles;
    }

    if (nInitialBeansPerHole < 0)
    {
        m_initialBeans = 0;
    }
    else
    {
        m_initialBeans = nInitialBeansPerHole;
    }

    // initializing values

    // creates the north and south side pots. Also conveniently fixed the indexes
    north_side.push_back(0);
    south_side.push_back(0);

    // filling the sides with beans
    for (int i = 0; i < m_holes; i++)
    {
        north_side.push_back(m_initialBeans); // adds the beans into the north side
        south_side.push_back(m_initialBeans); // adds the beans into the south side
    }
}

// Return the number of holes on a side (not counting the pot).
int Board::holes() const
{
    return m_holes;
}

// Return the number of beans in the indicated hole or pot,
// or −1 if the hole number is invalid.
int Board::beans(Side s, int hole) const
{
    if (hole < 0 || hole > m_holes) // invalid hole/pot number
    {
        return -1;
    }

    if (s == NORTH)
    {
        return north_side.at(hole);
    }
    else // s is SOUTH
    {
        return south_side.at(hole);
    }
}

// Return the total number of beans in all the holes on the indicated side,
// not counting the beans in the pot.
int Board::beansInPlay(Side s) const
{
    int count = 0;
    if (s == NORTH)
    {
        for (int i = 1; i <= m_holes; i++)
        {
            count += north_side.at(i);
        }
    }
    else // s == SOUTH
    {
        for (int i = 1; i <= m_holes; i++)
        {
            count += south_side.at(i);
        }
    }
    return count;
}

// Return the total number of beans in the game, including any in the pots.
int Board::totalBeans() const
{
    int beans = 0;
    for (int i = 0; i <= m_holes; i++) // starts by counting pots, then other holes
    {
        beans += north_side.at(i); // count bean on north
        beans += south_side.at(i); // count bean on south
    }
    return beans;
}

// If the hole indicated by (s,hole) is empty or invalid or a pot, this function returns
// false without changing anything. Otherwise, it will return true after sowing the beans:
// the beans are removed from hole (s,hole) and sown counterclockwise, including s's pot if encountered,
// but skipping s's opponent's pot. The function sets the parameters endSide and endHole to the side
//  and hole where the last bean was placed. (This function does not make captures or multiple turns;
// different Kalah variants have different rules about these issues, so dealing with them should not be
// the responsibility of the Board class.)
bool Board::sow(Side s, int hole, Side& endSide, int& endHole) // doesn't sow properly sometimes, puts 2 in the first hole or leaves one in the original hole
{
    // Checking validity:
    if (invalidHole(hole))
    {
        return false;
    }

    int beans;
    if (s == NORTH)
    {
        beans = north_side.at(hole); // get beans
        if (beans == 0)              // if beans are empty
        {
            return false;
        }
        else
        {
            north_side.at(hole) = 0;   // remove beans
            int start_drop = hole - 1; // first index where we drop a bean
            while (beans > 0)         // while we have beans
            {
                for (int i = start_drop; i > 0; i--) // while in north side
                {
                    north_side.at(i)++; // increase bean count
                    beans--;            // we lose a bean
                    if (beans == 0)     // if it was our last bean
                    {
                        // set endings
                        endHole = i;
                        endSide = Side::NORTH;
                        return true; // beans successfully sown
                    }
                }

                // move to north pot, and then continue
                if (beans > 0)
                {
                    north_side.at(0)++; // increase beans in pot
                    beans--;            // lose a bean to the pot
                    if (beans == 0)     // if it was our last bean
                    {
                        // set endings
                        endHole = POT;
                        endSide = Side::NORTH;
                        return true; // beans successfully sown
                    }
                }

                // need to move to south side
                start_drop = 1;     // reset the hole we will be dropping in for south side

                for (int i = start_drop; i <= m_holes; i++) // while in south side
                {
                    south_side.at(i)++; // increase bean count
                    beans--;            // we lose a bean
                    if (beans == 0)     // if it was our last bean
                    {
                        // set endings
                        endHole = i;
                        endSide = Side::SOUTH;
                        return true; // beans successfully sown
                    }
                }
                         
                // if we still have beans, loop back to beginning of while loop
                // note that we do NOT deposit in the south side's pot

                start_drop = m_holes; // resetting holes to go through north
            }
        }
        return true;
    }
    else // s == SOUTH
    {
        beans = south_side.at(hole); // get beans
        if (beans == 0)              // if beans are empty
        {
            return false;
        }
        else
        {
            south_side.at(hole) = 0;   // remove beans
            int start_drop = hole + 1; // first index where we drop a bean
            while (beans != 0)         // while we have beans
            {
                for (int i = start_drop; i <= m_holes; i++) // while in south side
                {
                    south_side.at(i)++; // increase bean count
                    beans--;            // we lose a bean
                    if (beans == 0)     // if it was our last bean
                    {
                        // set endings
                        endHole = i;
                        endSide = Side::SOUTH;
                        return true; // beans successfully sown
                    }
                }
                // need to move to south pot, and then to north side
                if (beans > 0)
                {
                    south_side.at(0)++;   // increase beans in pot
                    beans--;              // lose a bean to the pot
                    start_drop = m_holes; // reset the hole we will be dropping in for north side

                    if (beans == 0)     // if it was our last bean
                    {
                        // set endings
                        endHole = POT;
                        endSide = Side::SOUTH;
                        return true; // beans successfully sown
                    }
                }               

                for (int i = start_drop; i > 0; i--) // while in north side
                {
                    north_side.at(i)++; // increase bean count
                    beans--;            // we lose a bean
                    if (beans == 0)     // if it was our last bean
                    {
                        // set endings
                        endHole = i;
                        endSide = Side::NORTH;
                        return true; // beans successfully sown
                    }
                }
                // if we still have beans, loop back to beginning of while loop
                // note that we do NOT deposit in the north side's pot

                start_drop = 1; // resetting hole
            }
        }
        return true;
    }
}

// If the indicated hole is invalid or a pot, return false without changing anything.
// Otherwise, move all the beans in hole (s, hole) into the pot belonging to potOwner and return true.
bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    if (invalidHole(hole))
    {
        return false;
    }

    // get beans
    int beans;
    if (s == NORTH)
    {
        beans = north_side.at(hole);
        north_side.at(hole) = 0;
    }
    else // s == SOUTH
    {
        beans = south_side.at(hole);
        south_side.at(hole) = 0;
    }

    // move beans
    if (potOwner == NORTH)
    {
        north_side.at(POT) += beans;
    }
    else // potOwner == SOUTH
    {
        south_side.at(POT) += beans;
    }

    return true;
}

// If the indicated hole is invalid or beans is negative, this function returns false without changing
// anything. Otherwise, it will return true after setting the number of beans in the indicated hole
// or pot to the value of the third parameter. (This could change what beansInPlay and totalBeans return
// if they are called later.) This function exists solely so that we and you can more easily test your
// program: None of your code that implements the member functions of any class is allowed to call this
// function directly or indirectly. (We'll show an example of its use below.)
bool Board::setBeans(Side s, int hole, int beans)
{
    if (hole < 0 || hole > m_holes || beans < 0) // if invalid hole or negative beans
    {
        return false;
    }
    else
    {
        if (s == NORTH)
        {
            north_side.at(hole) = beans;
            return true;
        }
        else // s == SOUTH
        {
            south_side.at(hole) = beans;
            return true;
        }
    }
}

// helper functions:

// returns true if the hole is invalid, false if the hole is valid.
// note: will return true if it's a pot.
bool Board::invalidHole(int hole) const
{
    if (hole <= 0 || hole > m_holes) // if the hole is not holes 1 through number of holes, hole 0 is a pot
    {
        return true;
    }
    return false;
}