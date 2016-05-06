/*
Battleship Game
PROG FEUP
Ntagiou Anna
201409482
*/

#include <iostream>
#include <string>
#include <fstream> 
#include <vector>
#include <ctime>
#include <cstdlib>
#include <ctype.h>
#include <Windows.h>
#include<sstream>



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
//Typedef
typedef struct 
{
	unsigned int x;
	unsigned int y;
	char xChar;
	char yChar;
	char orientation;
}position;

typedef struct
{
	string name;
	unsigned int size;
	char symbol;
	unsigned int colorCode;
	position position;
}ship;

typedef vector <ship> fleet;
typedef vector<vector<char>> boardType;


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
// Position the cursor at column 'x', line 'y'
// The coodinates of upper left corner of the screen are (x,y)=(0,0)
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
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
// Configuration and Board file
bool isTheFileValid(string &filename)
{
	ifstream file;

	if (filename.find(".txt") == -1)
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
int getColorCode(string color)
{
	unsigned int i = 0;

	while (color != colors[i])
	{
		i++;
		if (i > 15) return 0;
	}
	return i;
}

//==========================================================================================
// Read the ships from the file and write them to the vector
void shipToVector(string filename, fleet &ships)
{
	ifstream configurationFile;
	unsigned int quantityOfShip, sizeOfShip, i=0, j;
	char  delimiter='-', symbolOfShip;
	string nameOfShip, colorOfShip;
	ship entryOfShip;

	configurationFile.open(filename);
	configurationFile.ignore(1000, '\n');
	
	while (!configurationFile.eof())
	{
		ships.push_back(entryOfShip);

		configurationFile >> quantityOfShip >> delimiter >> nameOfShip >> delimiter >> sizeOfShip >> delimiter >> symbolOfShip >> delimiter >> colorOfShip;

		ships.at(i).name = nameOfShip;
		ships.at(i).size = sizeOfShip;
		ships.at(i).symbol = symbolOfShip;
		ships.at(i).colorCode = getColorCode(colorOfShip);
		for (j = 0; j < quantityOfShip - 1; j++)
		{
			ships.push_back(ships.at(i));
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
	configurationFile.ignore(1000, ':');
	configurationFile >> rows;

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
	configurationFile.ignore(1000, 'x');
	configurationFile >> columns;

	configurationFile.close();
	return columns;
}

//==========================================================================================
// Initialization of board file 
void initBoardFile (string &boardFile)
{
	string continueAnswer;

	cout << "Enter the name of board file: "; cin >> boardFile;

	if (isTheFileValid(boardFile)){
		cout << "The name of board file is already exist. " << endl;
		cout << "Continue? (y/n) " << endl; cin >> continueAnswer;
		continueAnswer = tolower(continueAnswer.at(0));
		if (continueAnswer == "n")
			initBoardFile(boardFile);
	}
	else
	{
		ofstream createFile;
		if (boardFile.find(".txt") == -1)
			boardFile += ".txt";
		createFile.open(boardFile);
		createFile.close();
	}
}

//==========================================================================================
// Print the board on screen
void printBoard(const boardType &board, const fleet &ships)
{
	// Print the first horizontal line (row) with the lower letters
	setColor(15);
	for (unsigned int i = 0; i < board.at(0).size(); i++)	
	{		
		if (i == 0) cout << "  ";
		cout << ' ' << (char)(97 + i);						
	}														
	cout << endl;

	// Print the first vertical line (column) with the capital letters
	for (unsigned int i = 0; i < board.at(0).size(); i++)
	{
		setColor(15);
		cout << ' ' << (char)(65 + i);		
		//Print the board
		for (unsigned int j = 0; j < board.size(); j++)
		{
			//If there is not ship, print '.'
			if (board.at(i).at(j) == '.')
				setColor(1, 15);
			else
			{	//Else print the symbol of the ship with the specific color
				int k = 0;
				while (board.at(i).at(j) != ships.at(k).symbol)
				{
					k++;
				}
				setColor(ships.at(k).colorCode, getColorCode(boardColor));
			}
			cout << ' ' << board[i][j];
			/*if (i == board.size() || j == board.size())
			{
				setColor(15);
				cout << ' ';
			}
			*/
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

	finalPosition.y = y;
//	if (result.y > 25)
//		result.y = 25;
	finalPosition.yChar = (char)(y + 65);
	finalPosition.x = x;
//	if (result.x > 25)
//		result.x = 25;
	finalPosition.xChar = (char)(x + 97);

	if (orientation == 0)
		finalPosition.orientation = 'H';
	else
		finalPosition.orientation = 'V';

	return finalPosition;
}

//==========================================================================================
// Check if the position of the ship is valid
bool isThePositionValid(boardType &board, const position &position, unsigned int size)
{

	switch (position.orientation)
	{
	case 'H':
		if (position.x + size > board.at(0).size())
			return false;
		for (unsigned int i = 0; i < size; i++)
		{
			if (board.at(position.y).at(position.x + i) != '.')
			{
				return false;
			}
		}
		break;
	case 'V':
		if (position.y + size > board.size())
			return false;
		for (unsigned int i = 0; i < size; i++)
		{
			if (board.at(position.y + i).at(position.x) != '.')
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
bool addShipToBoard(boardType &board, const fleet &ships, int index)
{
	if (!isThePositionValid(board, ships.at(index).position, ships.at(index).size))
		return false;

	switch (ships.at(index).position.orientation)
	{
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
void automaticMode(boardType &board, fleet &ships)
{
	srand((unsigned int)time(NULL));

	for (unsigned int i = 0; i < ships.size(); i++)
	{
		do
		{
			unsigned int x, y, orientation;
			x = rand() % board.at(0).size();
			y = rand() % board.size();
			orientation = rand() % 2 == 0;

			ships.at(i).position = parseCoordinatesOrientation(x, y, orientation);
		} while (!addShipToBoard(board, ships, i));
	}

	clrScr();
	printBoard(board, ships);

}

//==========================================================================================
// Parse the coordinates
position parseCoordinates(string coords)
{
	position finalPosition;

	finalPosition.yChar = coords.at(0);
	finalPosition.y = (finalPosition.yChar - 65) % 32;
	if (finalPosition.y < 0 || finalPosition.y > 25)
	{
		finalPosition.yChar = 'A';
		finalPosition.y = 0;
	}
	finalPosition.xChar = coords.at(1);
	finalPosition.x = (finalPosition.xChar - 65) % 32;
	if (finalPosition.x < 0 || finalPosition.x > 25)
	{
		finalPosition.xChar = 'a';
		finalPosition.x = 0;
	}

	if (coords.size() > 2)
		finalPosition.orientation = toupper(coords.at(2));

	return finalPosition;
}

//==========================================================================================
// Get the quantity of every ship
int getTheQuantityOfShip(const fleet &ships, int index)
{
	unsigned int counter = 0;

	for (unsigned int i = index; i < ships.size(); i++)
	{
		if (ships.at(i).symbol == ships.at(index).symbol)
			counter++;
		else
			if (counter > 0)
				return counter;
	}

	return counter;
}

//==========================================================================================
// Place the ships in positions from the user
void manualMode(boardType &board, fleet &ships)
{
	string posOfNewShip;

	for (unsigned int i = 0; i < ships.size(); i++)
	{
		bool shipAdded = true;
		clrScr();
		printBoard(board, ships);
		cout << endl << endl;

		do
		{
			if (!shipAdded)
				cout << "Error! The position of the ship is not valid.\n";

			setColor(ships.at(i).colorCode);
			cout << ships.at(i).symbol << " - " << ships.at(i).name << ". Size = " << ships.at(i).size << ".  Available ships " << getTheQuantityOfShip(ships,i) << "\n" << endl;
			cout << "LINE (A..J) COLUMN (a..j) ORIENTATION (H V) ?" << endl;
			setColor(15);
			shipAdded = false;
			cin >> posOfNewShip;
			ships.at(i).position = parseCoordinates(posOfNewShip);
			clrScr();
			printBoard(board, ships);
		} while (!addShipToBoard(board, ships, i));
	}
	clrScr();
	printBoard(board, ships);
}

//==========================================================================================
// Write the contents of the board in the file 
void writeBoardInFile(const boardType &board, const fleet &ships, string filename)
{
	ofstream boardFile;

	boardFile.open(filename);

	boardFile << board.size() << " x " << board.at(0).size() << endl;

	for (unsigned int i = 0; i < ships.size(); i++)
	{
		boardFile << ships.at(i).symbol << ' ' << ships.at(i).size << ' ' << ships.at(i).position.xChar << ships.at(i).position.yChar << ' ' << ships.at(i).position.orientation << endl;
	}

	boardFile.close();
}

//==========================================================================================
// Main program 
int main(){
	
	string configurationFile, boardFile;
	fleet ships;
	string workingMode;

	cout << "Enter the name of configuration file: "; cin >> configurationFile;

	if (!isTheFileValid(configurationFile))
	{	
		cerr << "Error opening " << configurationFile << "The file is not found!"<< "\n";
		cerr << "System Exit" << "\n";
		system("pause");
		exit(1);
	}

	initBoardFile(boardFile);

	cout << "Enter the working mode: "; cin >> workingMode;
	workingMode = tolower(workingMode.at(0));
	cout << workingMode;
	while ((workingMode !="a") || (workingMode != "m"))
	{
		cout << "Enter the working mode: "; cin >> workingMode;
		workingMode = tolower(workingMode.at(0));
	}

	shipToVector(configurationFile, ships);
	
	const int boardRows = getBoardRows(configurationFile);
	const int boardColumns = getBoardColumns(configurationFile);

	boardType board(boardRows, vector<char>(boardColumns, '.'));

	system("pause");
	clrScr();
	printBoard(board, ships);

	if (workingMode == "a")
		automaticMode(board, ships);
	else 
		manualMode(board, ships);

	writeBoardInFile(board, ships, boardFile);

	system("pause");
	return 0;

}