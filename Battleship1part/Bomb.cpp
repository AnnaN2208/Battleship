#include "Bomb.h"
#include "structs.h"
//=============================================================================
Bomb::Bomb()
{

}

void Bomb::setTargetLine(char tL)
{
	targetLine = tL;
}

void Bomb::setTargetColumn(char tC)
{
	targetColumn = tC;
}

char Bomb::getTargetLine() const
{
	return targetLine;
}
char Bomb::getTargetColumn() const
{
	return targetColumn;
}
