/**
 *Battleship Game
 *PROG FEUP
 *Ntagiou Anna
 *up201409482
 *20-04-2015
 */

#include <iostream>
#include <string>
#include <fstream> 
#include <vector>
#include <ctime>
#include <cstdlib>
#include <ctype.h>
#include <Windows.h>
#include <sstream>
#include "structs.h"
#include "Ship.h"
#include "Board.h"
#include "Bomb.h"


using namespace std;

//==========================================================================================
//COLORS: 
const string colors[] = 
{
	"Black", "Blue", "Green", "Cyan", "Red", "Magenta",
	"Brown", "Light_gray", "Dark_gray", "Light_blue", "Dark_blue", "Light_green",
	"Light_cyan", "Light_red", "Light_magenta", "Yellow", "White"
};
const string boardColor = "Light_blue";


//==========================================================================================
// Clear screen
void clrScr(void)
{
	COORD coordScreen = { 0, 0 }; // upper left corner
	unsigned long cCharsWritten;
	unsigned long dwConSize;
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hCon, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	// fill with spaces
	FillConsoleOutputCharacter(hCon, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hCon, &csbi);
	FillConsoleOutputAttribute(hCon, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	// cursor to upper left corner
	SetConsoleCursorPosition(hCon, coordScreen);
}

//==========================================================================================
// Set text color
void setColor(unsigned int color)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}

//==========================================================================================
// Set text color & background
void setColor(unsigned int color, unsigned int background_color)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	if (background_color == 0)
		SetConsoleTextAttribute(hCon, color);
	else
		SetConsoleTextAttribute(hCon, color | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
}

//==========================================================================================
// Check if the file is valid
// Used for configuration and board file
bool isTheFileValid(string &filename)
{
	ifstream file;

	if (filename.find(".txt") == -1) //If user enter only the filename, it adds the .txt 
		filename += ".txt";

	file.open(filename);

	if (file.fail()) // OR if (! infile.is_open()) OR if (! infile)
	{
		return false;
	}

	file.close();
	return true;
}

//==========================================================================================
// Get the color code of the color name
// This function compares the color name with the constant array of strings "colors"
// and returns the color code
int getColorCode(string color)
{
	unsigned int i = 0;

	while (color != colors[i])
	{
		i++;
		if (i > 15) return 0; //if the index of the color name is more than 15, it returns black(0).
	}

	return i;
}

//==========================================================================================
// Read the ships from the file and write them to the vector
void shipToVector(string filename, Board &ships)
{
	ifstream configurationFile;
	unsigned int quantityOfShip, sizeOfShip, i=0, j;
	char  delimiter='-', symbolOfShip;
	string nameOfShip, colorOfShip;

	configurationFile.open(filename);
	configurationFile.ignore(1000, '\n');	//ignore the first line
	
	while (!configurationFile.eof())		//if it is not the end of the file
	{
		Ship newship;
		//reading line of the file
		configurationFile >> quantityOfShip >> delimiter >> nameOfShip >> delimiter >> sizeOfShip >> delimiter >> symbolOfShip >> delimiter >> colorOfShip; 
		
		
		//fill the ship with values
		newship.setColor(getColorCode(colorOfShip));
		newship.setSymbol(symbolOfShip);
		newship.setSize(sizeOfShip);

		//fill the vector with ships that depends on the quantity
		for (j = 0; j < quantityOfShip - 1; j++)
		{
			//ships.push_back(ships.at(i));
			ships.addShip(newship);
		}
		i += quantityOfShip;
	}
	configurationFile.close();
}

//==========================================================================================
// Get the rows of the board from the configuration file 
int getBoardRows(string filename)
{
	ifstream configurationFile;
	int rows;

	configurationFile.open(filename);
	configurationFile.ignore(1000, ':'); //ignore the ':' character
	configurationFile >> rows;			 //and after this, there are the rows

	configurationFile.close();
	return rows;
}

//==========================================================================================
// Get the columns of the board from the configuration file 
int getBoardColumns(string filename)
{
	ifstream configurationFile;
	int columns;

	configurationFile.open(filename);
	configurationFile.ignore(1000, 'x'); //ignore the 'x' character
	configurationFile >> columns;		 //and after this, there are the columns

	configurationFile.close();
	return columns;
}

//==========================================================================================
// Initialization of board file 
void initBoardFile (string &boardFile)
{
	string continueAnswer;

	cout << "Enter the name of board file: "; cin >> boardFile;

	if (isTheFileValid(boardFile))
	{	//case that the file is already exists
		cout << "The name of board file is already exist. " << endl;
		cout << "Continue? (y/n) " << endl; cin >> continueAnswer;
		continueAnswer = tolower(continueAnswer.at(0));
		//if the user don´t want to continue with the same file, call the same function again
		if (continueAnswer == "n")
			initBoardFile(boardFile);
	}
	else
	{	//case that the  file is not already exists
		ofstream createFile;
		if (boardFile.find(".txt") == -1)	//if the user doesn´t enter .txt
			boardFile += ".txt";			//the program add the .txt for the text file
		createFile.open(boardFile);
		createFile.close();
	}
}

//==========================================================================================
// Print the board on screen
void printBoard(Board &board, Board &ships)
{
	// Print the first horizontal line (row) with the lower letters
	setColor(15);  //set the color of the letters
	for (unsigned int i = 0; i < board.getNumColumns; i++)	
	{		
		if (i == 0) cout << "  ";		
		cout << ' ' << (char)(97 + i);	 //the ASCII characters for lower letters are from 97 to 122					
	}														
	cout << endl;

	// Print the first vertical line (column) with the capital letters
	for (unsigned int i = 0; i < board.getNumColumns; i++)
	{
		setColor(15); //set the color of the letters
		cout << ' ' << (char)(65 + i);	//the ASCII characters for capital letters are from 65 to 90		
		//Print the board
		for (unsigned int j = 0; j < board.getNumLines; j++)
		{
			//If there is not ship, print '.'
			if (board.at(i).at(j) == '.')
				setColor(1, 15);  //Set text color of '.' (1) & background(15)
			else
			{	//Else print the symbol of the ship with the specific color
				int k = 0;
				while (board.at(i).at(j) != ships.at(k).symbol)
				{
					k++;
				}
				setColor(ships.at(k).colorCode, getColorCode(boardColor)); //Set text color of the ship & background
			}
			cout << ' ' << board[i][j];
		}
		cout << endl;
	}
	setColor(15);
}

//==========================================================================================
// Parse the coordinates and orientation
position parseCoordinatesOrientation(unsigned int x, unsigned int y, unsigned int orientation)
{
	position finalPosition;

	finalPosition.y = y;					//y coordinate of position in int
	if (finalPosition.y > 25)				//case that the the y coordinate is another character that [A..Z] or [a..z]
		finalPosition.y = 25;
	finalPosition.yChar = (char)(y + 65);   //y coordinate of position in char
	finalPosition.x = x;					//x coordinate of position in int
	if (finalPosition.x > 25)				//case that the the x coordinate is another character that [A..Z] or [a..z]
		finalPosition.x = 25;
	finalPosition.xChar = (char)(x + 97);   //x coordinate of position in char


	if (orientation == 0)					//if the orientation is 0 then the ship will be placed horizontally
		finalPosition.orientation = 'H';
	else									//else it will be placed vertically
		finalPosition.orientation = 'V';

	return finalPosition;
}

//==========================================================================================
// Check if the position of the ship is valid
bool isThePositionValid(Board &board, const position &position, unsigned int size)
{

	switch (position.orientation)
	{
	case 'H':  //check valid position for horizontal orientation
		if (position.x + size > board.at(0).size())   //check the board limits
			return false;
		for (unsigned int i = 0; i < size; i++)
		{
			if (board.at(position.y).at(position.x + i) != '.') //check if there is another ship in the position of the new ship
			{
				return false;
			}
		}
		break;
	case 'V':  //check valid position for vertical orientation
		if (position.y + size > board.size())     //check the board limits
			return false;
		for (unsigned int i = 0; i < size; i++)
		{
			if (board.at(position.y + i).at(position.x) != '.')    //check if there is another ship in the position of the new ship
			{
				return false;
			}
		}
		break;
	}
	return true;
}

//==========================================================================================
// Add the ships in the board
bool addShipToBoard(Board &board, Board &ships, int index)
{
	if (!isThePositionValid(board, ships.at(index).position, ships.at(index).size))  //check if the porition of the new ship is valid
		return false;

	switch (ships.at(index).position.orientation)
	{	//depending on the orientation, place the symbol of the ship in the appropriate coordinates 
		case 'H':
		{
			for (unsigned int i = 0; i < ships.at(index).size; i++)
			{
				board.at(ships.at(index).position.y).at(i + ships.at(index).position.x) = ships.at(index).symbol;
			}
			break;
		}
		case 'V':
		{
			for (unsigned int i = 0; i < ships.at(index).size; i++)
			{
				board[i + ships.at(index).position.y][ships.at(index).position.x] = ships.at(index).symbol;
			}
			break;
		}
	}
	return true;
}

//==========================================================================================
// Place the ships in random positions
void automaticMode(Board &board, Board &ships)
{
	srand((unsigned int)time(NULL));

	for (unsigned int i = 0; i < ships.size(); i++)
	{
		do
		{
			unsigned int x, y, orientation;
			x = rand() % board.getNumColumns;  //generate random x coordinate
			y = rand() % board.getNumLines;	      //generate random y coordinate
			orientation = rand() % 2 == 0;    //generate random orientaion (0 or 1)

			ships.at(i).position = parseCoordinatesOrientation(x, y, orientation);  //fill the position of the ship in the vector
		} while (!addShipToBoard(board, ships, i));
	}
	//clear screen and print the new board
	clrScr();				
	printBoard(board, ships);

}

//==========================================================================================
// Parse the coordinates
position parseCoordinates(string coords)
{
	position finalPosition;

	finalPosition.yChar = coords.at(0);					//first character of user entry
	finalPosition.y = (finalPosition.yChar - 65) % 32;  //casting the character to int regardless of the case of the letter
	if (finalPosition.y < 0 || finalPosition.y > 25)	//if the user have entered another character that [A..Z] or [a..z]
	{
		finalPosition.yChar = 'A';						//the y coordinate will be 'A'
		finalPosition.y = 0;
	}
	finalPosition.xChar = coords.at(1);					//second character of user entry
	finalPosition.x = (finalPosition.xChar - 65) % 32;  //casting the character to int regardless of the case of the letter
	if (finalPosition.x < 0 || finalPosition.x > 25)	//if the user has entered another character that [A..Z] or [a..z]
	{
		finalPosition.xChar = 'a';						//the y coordinate will be 'a'
		finalPosition.x = 0;
	}

	if (coords.size() > 2)								//fill the orientation in case that the user has entered more than the coordinates
		finalPosition.orientation = toupper(coords.at(2));

	return finalPosition;
}

//==========================================================================================
// Get the quantity of every ship
int getTheQuantityOfShip(Board &ships, int index)
{
	unsigned int counter = 0;

	for (unsigned int i = index; i < ships.size(); i++)
	{
		if (ships.at(i).symbol == ships.at(index).symbol)	//find the ship by the symbol and the index
			counter++;										//and increase the quantity
		else
			if (counter > 0) return counter;
	}

	return counter;
}

//==========================================================================================
// Place the ships in positions from the user
void manualMode(Board &board, Board &ships)
{
	string posOfNewShip;

	for (unsigned int i = 0; i < ships.size(); i++)
	{
		bool shipAdded = true;
		clrScr();
		printBoard(board, ships);

		do
		{
			if (!shipAdded)
				cout << "Error! The position of the ship is not valid.\n";
			//print the information of the ships to the user and ask for the position
			setColor(ships.at(i).colorCode);
			cout << "\n" << ships.at(i).symbol << " - " << ships.at(i).name << ". Size = " << ships.at(i).size << ".  Available ships " << getTheQuantityOfShip(ships,i) << "\n" << endl;
			cout << "LINE (A..J) COLUMN (a..j) ORIENTATION (H V) ?" << endl;
			setColor(15);
			shipAdded = false;
			cin >> posOfNewShip;
			ships.at(i).position = parseCoordinates(posOfNewShip); //fill the new position of the ship in the vector
			//clear screen and print the new board
			clrScr();
			printBoard(board, ships);
		} while (!addShipToBoard(board, ships, i));
	}
	//clear screen and print the new board
	clrScr();
	printBoard(board, ships);
}

//==========================================================================================
// Write the contents of the board in the file 
void writeBoardInFile(Board &board, Board &ships, string filename)
{
	ofstream boardFile;

	boardFile.open(filename);	//open the board file

	boardFile << board.getNumLines << " x " << board.getNumColumns << endl;  //write the dimensions of the board

	//write the positions of the ships
	for (unsigned int i = 0; i < ships.size(); i++)
	{
		boardFile << ships.at(i).symbol << ' ' << ships.at(i).size << ' ' << ships.at(i).position.xChar << ships.at(i).position.yChar << ' ' << ships.at(i).position.orientation << ' ' << ships.at(i).colorCode << endl;
	}

	boardFile.close();
}

//==========================================================================================
// Main program 
int main(){
	
	string configurationFile, boardFile;
	Board ships;
	char workingMode;

	cout << "Enter the name of configuration file: "; cin >> configurationFile;

	if (!isTheFileValid(configurationFile))  //if the configuration file doesn´t exist, exit the program
	{	
		cerr << "Error opening " << configurationFile << "The file is not found!"<< "\n";
		cerr << "System Exit" << "\n";
		exit(1);
	}

	initBoardFile(boardFile);  //ask for the board file from the user

	cout << "Enter the working mode ( [a,A] = automatic, [m,M] = manual ): "; cin >> workingMode;
	cout << workingMode << endl;

	switch (workingMode)  //if the working mode isn´t valid, exit the program
	{
	case 'a':
	case 'A':
		cout << "You enter the automatic mode." << endl;
		break;
	case 'M':
	case 'm':
		cout << "You enter the manual mode." << endl;
		break;
	default:
		cerr << "You enter invalid working mode." << endl;
		cerr << "System Exit" << "\n";
		system("pause");
		exit(1);
	}
	

	shipToVector(configurationFile, ships);  //fill the ship vector with ships from the configuration file
	
	//set constant the dimension of the board
	const int boardRows = getBoardRows(configurationFile);			
	const int boardColumns = getBoardColumns(configurationFile);

	Board boardA;
	boardA.setNumLines = boardRows;
	boardA.getNumColumns = boardColumns;
	
	//clear screen and print the new board
	clrScr();
	printBoard(boardA, ships);

	//cotinue of the game depending the working mode
	if (workingMode == 'a' || (workingMode == 'A'))
		automaticMode(boardA, ships);  
	else 
		manualMode(boardA, ships);

	writeBoardInFile(boardA, ships, boardFile);  //write the dimmension and the positions of the ships in the board file

	system("pause");
	return 0;

}