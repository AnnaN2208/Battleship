#include "Ship.h"
#include "structs.h"
//=============================================================================

IdentNumShip Ship::numShips = 0; //static variable definition and initialization

Ship::Ship() 
{
	numShips++;
	idShip = numShips;
}

void Ship::setSymbol(char s)
{
	symbol = s;
}

void Ship::setPosition(PositionChar pos)
{
	position = pos;
}

void Ship::setOrientation(char or)
{
	orientation = or;
}

void Ship::setSize(unsigned int si)
{
	size = si;
}

void Ship::setColor(unsigned int col)
{
	color = col;
}


char Ship::getSymbol() const
{
	return symbol;
}


PositionChar Ship::getPosition() const
{
	return position;
}

char Ship::getOrientation() const
{
	return orientation;
}

unsigned int Ship::getSize() const
{
	return size;
}

unsigned int Ship::getColor() const
{
	return color;
}

IdentNumShip Ship::getIdShip() const
{
	return idShip;
}