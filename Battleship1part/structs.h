#ifndef STRUCTS_H
#define STRUCTS_H

typedef unsigned long IdentNumShip;
//-----------------------------------------------------------------------------
struct PositionChar // to store a board position in char format
{ // example: 'B','d'
	char lin, col; // ranges: lin - ['A'..'Z']; col - ['a'..'z']
};
//-----------------------------------------------------------------------------
struct PositionInt  // to store a board position in unsig. integer format
{ // example: 7,0
	unsigned int lin, col; // ranges: lin - [0..26]; col - [0..26]
};

// This is the end of the header guard
#endif