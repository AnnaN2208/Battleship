#ifndef BOARD_H
#define BOARD_H
#include <string>
#include "Bomb.h"
#include "Ship.h"
#include <vector>

using namespace std;

class Board
{
public:
	Board(const string &filename); // loads board from file 'filename'
	bool putShip(const Ship &s); // adds ship to the board, if possible
	void moveShips(); // tries to randmonly move all the ships of the fleet
	bool attack(const Bomb &b);
	void display() const; // displays the colored board during the game
	void show() const; // shows the attributes of the board (for debugging)
	void addShip(Ship sh);
	
	Board::Board();
	void setNumLines(int nL);
	void setNumColumns(int nC);

	int getNumLines() const;
	int getNumColumns() const;
	
private:
	int numLines, numColumns; // redundant info …
	vector<Ship> ships; // ships that are placed on the board
	vector<vector<int>> board; // each element indicates
	// the index of a ship in the 'ships' vector
	// (in the range 0..ships.size()-1) ;
	// -1 is used to represent the sea
};
#endif