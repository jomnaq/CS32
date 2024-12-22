#ifndef SIDE_H
#define SIDE_H

enum Side
{
	NORTH,
	SOUTH
};

const int NSIDES = 2;
const int POT = 0;
const int MAX_TIME = 4990; // in ms

inline Side opponent(Side s)
{
	return Side(NSIDES - 1 - s);
}


// TIMER CLASSES

//========================================================================
// Timer t;                 // create and start a timer
// t.start();               // restart the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#include <chrono>

class Timer
{
public:
    Timer();
    void start();
    double elapsed() const;
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

// Advance jumpy timer only after jumpInterval calls to elapsed
class JumpyTimer
{
public:
	JumpyTimer(int jumpInterval);
	double elapsed();
	double actualElapsed();
private:
	Timer m_timer;
	int m_jumpInterval;
	int m_callsMade;
	int m_lastElapsed;
};

#endif // SIDE_H