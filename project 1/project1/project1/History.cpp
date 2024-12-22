#include "History.h"
#include "Arena.h"
#include "globals.h"

using namespace std;

History::History(int nRows, int nCols) : rows(nRows), cols(nCols) {
	for (int r = 1; r <= rows; r++) {
		for (int c = 1; c <= cols; c++) {
			historyGrid[r-1][c-1] = '.'; // draws out the grid as 2D array
		}
	}
}

bool History::record(int r, int c) {
	// check if parameters are valid
	if (r > rows || r <= 0 || c > cols || c <= 0) {
		return false;
	}

	// increases number of rabbits poisoned at that point
	if (historyGrid[r-1][c-1] == '.') { // if it's the first time, set to A (which is 1)
		historyGrid[r-1][c-1] = 'A';
	}
	else if (historyGrid[r - 1][c - 1] == 'Z') {
		// do nothing
	}
	else {
		historyGrid[r-1][c-1]++; // increments the character
	}
	
	return true;
}

void History::display() const {
	clearScreen();
	//prints out the grid
	for (int r = 1; r <= rows; r++) {
		for (int c = 1; c <= cols; c++) {
			cout << historyGrid[r-1][c-1];
		}
		cout << endl;
	}
	cout << endl;
}
