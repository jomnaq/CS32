#ifndef ARENA_H
#define ARENA_H

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>

#include "globals.h"
#include "History.h"

using namespace std;

class Rabbit;
class Player;

class Arena
{
public:
    // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

    // Accessors
    int rows() const;
    int cols() const;
    Player* player() const;
    int rabbitCount() const;
    int getCellStatus(int r, int c) const;
    int numberOfRabbitsAt(int r, int c) const;
    void display(string msg) const;
    History& history(); // Added History getter function

    // Mutators
    void setCellStatus(int r, int c, int status);
    bool addRabbit(int r, int c);
    bool addPlayer(int r, int c);
    void moveRabbits();

private:
    int     m_grid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Rabbit* m_rabbits[MAXRABBITS];
    int     m_nRabbits;
    int     m_turns;
    History m_history; // Added History data member

    // Helper functions
    void checkPos(int r, int c, string functionName) const;
    bool isPosInBounds(int r, int c) const;
};

#endif