#include "Board.h"
//=============================================================================
Board::Board()
{

}

void Board::setNumLines(int nL)
{
	numLines = nL;
}

void Board::setNumColumns(int nC)
{
	numColumns = nC;
}

int Board::getNumLines() const
{
	return numLines;
}

int Board::getNumColumns() const
{
	return numColumns;
}

void Board::addShip(Ship sh)
{
	ships.push_back(sh);
}


