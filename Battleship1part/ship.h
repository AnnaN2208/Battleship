#ifndef SHIP_H
#define SHIP_H
#include <string>
#include "structs.h"

using namespace std;

// This is the content of the .h file, which is where the declarations go
class Ship // function prototype for add.h -- don't forget the semicolon!
{
private:
	char symbol;
	PositionChar position;
	char orientation;
	unsigned int size;
	unsigned int color;
	string status;
	IdentNumShip idShip;

	static IdentNumShip numShips;

public:
	Ship(char symbol, PositionChar position, char orientation, unsigned int size, unsigned int color);

	Ship::Ship();
	void setSymbol(char sy);
	void setPosition(PositionChar pos);
	void setOrientation(char or);
	void setSize(unsigned int si);
	void setColor(unsigned int col);

	char getSymbol() const;
	PositionChar getPosition() const;
	char getOrientation() const;
	unsigned int getSize() const;
	unsigned int getColor() const;
	IdentNumShip getIdShip() const;

	bool move(char direction, bool rotate, unsigned int lineMin, unsigned int columnMin, unsigned int lineMax, unsigned int columnMax); // moves the boat (SEE NOTES)
	bool moveRand(unsigned int lineMin, unsigned int columnMin, unsigned int lineMax, unsigned int columnMax); // moves the ship randomly
	bool attack(size_t partNumber); //partNumber = {0,1,…, size-1}
	bool isDestroyed() const; // checks whether the ship is destroyed
	void show() const; // shows the attributes of the ship (for debugging)
};
// This is the end of the header guard
#endif