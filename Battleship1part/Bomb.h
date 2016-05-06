#ifndef BOMB_H
#define BOMB_H
#include <string>
#include "structs.h"

using namespace std;

class Bomb
{
public:
	Bomb(PositionChar targetPosition);
	PositionChar getTargetPosition() const;

	Bomb::Bomb();
	void setTargetLine(char tL);
	void setTargetColumn(char tC);

	char getTargetLine() const;
	char getTargetColumn() const;

	void show() const; // shows the attributes of the bomb (for debugging)

private:
	char targetLine, targetColumn;
};
#endif