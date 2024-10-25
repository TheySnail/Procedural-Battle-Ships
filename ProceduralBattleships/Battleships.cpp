#include "BattleShips.h"



int main()
{
	srand(time(0));
	bool exitFlag = false;
	int choice = 0;
	bool debugMap = false;
	bool debugAmmo = false;


	while (exitFlag == false)
	{
		system("cls");
		choice = Menu();
		switch (choice)
		{
		case 1: playGame(debugMap, debugAmmo);
			break;
		case 2: instructions();
			break;
		case 3: exitFlag = true;
			std::cout << "Thanks for playing";
			break;
		case 4: debugMode(debugMap, debugAmmo);
			break;
		default: std::cout << "invalid data inputted";
			break;
		}

	}
	return 0;
}

int Menu(void)
{
	int option{ 0 };
	bool menuExit{ false };
	while (menuExit == false)
	{
		system("cls");
		std::cout << "  BATTLESHIPS" << std::endl;
		std::cout << "***************" << std::endl;
		std::cout << "1. Play" << std::endl;
		std::cout << "2. Instructions" << std::endl;
		std::cout << "3. Quit" << std::endl;
		std::cout << "4. Debug Mode" << std::endl;
		std::cin >> option;
		if (option > 0 && option <= 4)//needs to between 1 and 4
		{
			menuExit = true;
		}
		else
		{
			system("cls");
			std::cout << "invalid option : press enter to continue " << std::endl;
			std::cin.clear();
			std::cin.ignore(100, '\n');
			std::cin.get();

		}
	}
	return option;

}

void playGame(bool _debugMap, bool _debugAmmo)
{

	char map[10][10];
	char displayMap[10][10]{ '~' };
	bool hasWon = false;
	int hitA = 0;
	int hitB = 0;
	int hitD = 0;// variables to keep track of how many of each ship has been hit
	int hitS = 0;
	int hitP = 0;
	int shipsDestroyed = 0;// amount of ships destroyed
	initaliseMap(map, displayMap);
	initaliseShip(map);
	int Guesses = 40;

	while (Guesses > 0 && hasWon == false)
	{
		system("cls");
		int xGuess = 0;
		int yGuess = 0;
		char rowInput[10];
		bool rowInputValid = false;
		bool colInputValid = false;
		printMap(map, displayMap, Guesses, _debugAmmo, _debugMap);

		while (rowInputValid == false)
		{
			std::cout << "What row do you want to shoot at? (A-J)" << std::endl;
			std::cin >> rowInput;
			if (strlen(rowInput) == 1)//checsk to see if more than one char is inputted
			{
				if (!isalpha(rowInput[0]))//this checks to make sure that the input is a letter not a number
				{
					std::cout << "Please enter a letter for the row you want to shoot at.";
					Sleep(1500);
					system("cls");
					printMap(map, displayMap, Guesses, _debugAmmo, _debugMap);
				}
				else
				{
					if (islower(rowInput[0]))//makes sure that the letter is in capital not lowercase
					{
						rowInput[0] = toupper(rowInput[0]);
					}
					if (!(static_cast<int>(rowInput[0]) >= 65 && static_cast<int>(rowInput[0]) < 75))//if not in the range of a-j (this is the ascii values)
					{
						//if the input is not between A and J the input is invalid and the loop variable is set to false 
						std::cout << "Please enter a letter between A and J";
						Sleep(1500);
						system("cls");
						printMap(map, displayMap, Guesses, _debugAmmo, _debugMap);
						rowInputValid = false;
					}
					else
					{
						if (std::cin.fail())//if the cin fails for whatever reason 
						{
							std::cout << "Invalid Data inputted please try again.";
							std::cin.clear();
							std::cin.ignore(1000, '\n');
						}
						else//this will happen if the input is in the correct format
						{
							rowInputValid = true;
						}
					}
				}
			}
			else//if more than one character is inputted this message plays
			{
				std::cout << "Please only enter one letter.";
				Sleep(1500);
				system("cls");
				printMap(map, displayMap, Guesses, _debugAmmo, _debugMap);
				rowInputValid = false;
			}
		}
		yGuess = (static_cast<char>(rowInput[0]) - 65);//converts the guess into a number between 0 and 9 

		while (colInputValid == false)
		{
			std::cout << "What column do you want to shoot at? (1-10)" << std::endl;
			std::cin >> xGuess;
			if (!(xGuess >= 1 && xGuess <= 10))//checks to see if the guess is not withing the range of 1-10
			{
				std::cout << "Please enter a number between 1 and 10";
				Sleep(1500);
				system("cls");
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				printMap(map, displayMap, Guesses, _debugAmmo, _debugMap);//clears screen and reprints the map
				colInputValid = false;
				std::cout << "Please enter a letter between A and J" << std::endl << rowInput << std::endl;// this reprints the users row guess so they can remember where they were shooting
			}
			else
			{
				if (std::cin.fail())//if the input fails this happens
				{
					std::cout << "Invalid Data inputted please try again." << std::endl;
					std::cin.clear();
					std::cin.ignore(1000, '\n');

				}
				else
				{
					colInputValid = true;
				}
			}

		}
		xGuess = xGuess - 1;//this adjusts for arrays starting at 0

		if (hasHit(map, displayMap, xGuess, yGuess, hitA, hitB, hitD, hitS, hitP) && _debugAmmo == false)//if debug ammo is on the players guesses wont be drained
		{
			Guesses--;//if the shot was valid the player looses ammo
		}
		if (gameWon(hitA, hitB, hitD, hitS, hitP, shipsDestroyed))
		{
			hasWon = true;
			std::cout << std::endl << "You sunk all the ships! You win!";
		}
		//needs noise for guesses
		Sleep(1500);
	}
	if (Guesses == 0)//if the player runs out of guess the lose message plays
	{
		system("cls");
		printMap(map, displayMap, Guesses, _debugAmmo, _debugMap);//clears screen and reprints the map
		std::cout << "Sorry you ran out of guesses, better luck next time!";
		Sleep(1500);
	}

	std::cin.get();
}

bool hasHit(char _map[][10], char _displayMap[10][10], int _xGuess, int _yGuess, int& hitA, int& hitB, int& hitD, int& hitS, int& hitP)
{

	if (_displayMap[_xGuess][_yGuess] == '~')
	{
		switch (_map[_xGuess][_yGuess])// compares where the player shot to the map with ships, checking what is displayed there
		{
		case 'A':
			std::cout << "hit";
			++hitA;//increases the amount of times this specific ship has been hit (used in hasWon function)
			_displayMap[_xGuess][_yGuess] = 'X';//changes the display map to an X
			Beep(800, 750);//makes a high pitch beep for when the player hits
			break;

		case 'B':
			std::cout << "hit";
			++hitB;
			_displayMap[_xGuess][_yGuess] = 'X';
			Beep(800, 750);
			break;
		case 'D':
			std::cout << "hit";
			++hitD;
			_displayMap[_xGuess][_yGuess] = 'X';
			Beep(800, 750);
			break;
		case 'S':
			std::cout << "hit";
			++hitS;
			_displayMap[_xGuess][_yGuess] = 'X';
			Beep(800, 750);
			break;

		case 'P':
			std::cout << "hit";
			++hitP;
			_displayMap[_xGuess][_yGuess] = 'X';
			Beep(800, 750);
			break;
		default:
			std::cout << "Miss";
			_displayMap[_xGuess][_yGuess] = 'O';
			Beep(120, 750);//low pitch beep for miss
			break;
		}
		std::cout << std::endl;
		return true;//returns true as the player has made a shot that removes ammo
	}
	else
	{
		std::cout << "You already Chosen there!" << std::endl;//if the space is not a ~ it means its eitehr an X or O meaning they have already shot there
		std::cout << std::endl;
		return false;//return false because the player has not made a shot which should remove ammo
	}
}

bool gameWon(int& _A, int& _B, int& _D, int& _S, int& _P, int& _shipsDestroyed)//checks the amount of ships that have been destroyed and if its equal to 5 the game is won
{
	if (_A == 5)
	{
		std::cout << "You destroyed my Aircraft Carrier";
		_A = 0;//needs to set varaible to 0 to stop it printing out You destroyed my xxxxxx again
		_shipsDestroyed++;//adds one to the amount of ships destroyed
	}
	if (_B == 4)
	{
		std::cout << "You destroyed my Battleship";
		_B = 0;
		_shipsDestroyed++;
	}
	if (_D == 3)
	{
		std::cout << "You destroyed my Destroyer";
		_D = 0;
		_shipsDestroyed++;
	}
	if (_S == 2)
	{
		std::cout << "You destroyed my Submarine";
		_S = 0;
		_shipsDestroyed++;
	}
	if (_P == 1)
	{
		std::cout << "You destroyed my Patrol Boat";
		_P = 0;
		_shipsDestroyed++;
	}

	if (_shipsDestroyed == 5)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void initaliseMap(char _map[][10], char _displaymap[][10])
{
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			_map[y][x] = '~';//initalises the map with ships in it
		}
	}
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			_displaymap[y][x] = '~';// intalises the map which is displayed
		}
	}
}

void initaliseShip(char _map[][10])
{
	bool lengthfit = true;
	// xPos goes across and yPos goes down so array is map[y][x] its confusing

	for (int length = 1; length <= 5; length++)//will run for each length of ship
	{

		bool locationValid = false;
		int direction = 1 + (rand() % 4);//random gen 1 to 4 for each 4 different directions


		while (locationValid == false)
		{
			char shipName;
			switch (length)//decides what the ships will be called based on length
			{
			case 1:
			{
				shipName = 'P';
				break;
			}
			case 2:
			{
				shipName = 'S';
				break;
			}
			case 3:
			{
				shipName = 'D';
				break;
			}
			case 4:
			{
				shipName = 'B';
				break;
			}
			case 5:
			{
				shipName = 'A';
				break;
			}
			}
			int xPos = rand() % 10;//generates random locations on grid
			int yPos = rand() % 10;
			switch (direction)//looks at the direction and sends flow to the correct direction gen 
			{
			case 1://ship gen to the right
			{
				bool isGood = false;
				if (yPos + length <= 9)//if the location + the length is greater than 9 it wont fit in the array
				{

					for (int j = 0; j <= length; j++)//will run for length of ship
					{
						if (_map[yPos + j][xPos] == '~')//checks to see if the current space is free
						{
							isGood = true;//if it is free bool set to true
						}
						else
						{
							isGood = false;//if a single one is taken the function returns false and stops
							j = 10;
						}
					}

				}
				if (isGood == true)//if the space is valid
				{
					locationValid = true;
					for (int j = 0; j < length; j++)
					{
						_map[yPos + j][xPos] = shipName;//place ship in the allocated spaces
					}
				}
				break;
			}
			case 2://ship gen to the left
			{
				bool isGood = false;
				if (yPos - length >= 0)//if the location + the length is greater than 9 it wont fit in the array
				{

					for (int j = 0; j <= length; j++)//will run for length of ship
					{
						if (_map[yPos - j][xPos] == '~')//checks to see if the current space is free
						{
							isGood = true;//if it is free bool set to true
						}
						else
						{
							isGood = false;//if a single one is taken the function returns false and stops
							j = 10;
						}
					}
				}
				if (isGood == true)
				{
					locationValid = true;
					for (int j = 0; j < length; j++)
					{
						_map[yPos - j][xPos] = shipName;
					}
				}
				break;
			}
			case 3://ship gen going up
			{
				if (xPos + length <= 9)//if the location + the length is greater than 9 it wont fit in the array
				{
					bool isGood = false;
					for (int j = 0; j <= length; j++)
					{
						if (_map[yPos][xPos + j] == '~')//checks to see if the current space is free
						{
							isGood = true;//if it is free bool set to true
						}
						else
						{
							isGood = false;//if a single one is taken the function returns false and stops
							j = 10;
						}
					}
					if (isGood)//bool outside of for loop as it needs to check all of them
					{
						locationValid = true;
						for (int j = 0; j < length; j++)
						{
							_map[yPos][xPos + j] = shipName;
						}
					}
				}
			}
			case 4://ship gen going down
			{
				if (xPos - length >= 0)//if the location + the length is greater than 9 it wont fit in the array
				{
					bool isGood = false;
					for (int j = 0; j <= length; j++)
					{
						if (_map[yPos][xPos - j] == '~')//checks to see if the current space is free
						{
							isGood = true;//if it is free bool set to true
						}
						else
						{
							isGood = false;//if a single one is taken the function returns false and stops
							j = 10;
						}
					}
					if (isGood)//bool outside of for loop as it needs to check all of them
					{
						locationValid = true;
						for (int j = 0; j < length; j++)
						{
							_map[yPos][xPos - j] = shipName;
						}
					}
				}
				break;
			}
			}
		}
	}

}

void printMap(char _map[][10], char _displayMap[][10], int _guesses, bool _debugAmmo, bool _debugMap)
{
	std::cout << "   1  2  3  4  5  6  7  8  9  10";
	if (_debugMap == true)// will print out the numbers for the debug map if it is on
	{
		std::cout << "\t";
		std::cout << "   1  2  3  4  5  6  7  8  9  10" << std::endl;
	}
	else// if debug map isnt on it needs to print out a line for the map to start prinitng
	{
		std::cout << std::endl;
	}
	for (int x = 0; x < 10; x++)
	{
		std::cout << static_cast <char> (65 + x) << "  ";//prints out the letters on the side using ascii values

		for (int y = 0; y < 10; y++)
		{
			std::cout << _displayMap[y][x] << "  ";
		}

		// if the debug map is on it indents and prints out the map with the ships on it
		if (_debugMap == true)
		{
			std::cout << "\t";
			std::cout << static_cast <char> (65 + x) << "  ";//prints out the letters on the side using ascii values
			for (int y = 0; y < 10; y++)
			{
				std::cout << _map[y][x] << "  ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	//ammo print out
	if (_debugAmmo == false)//if no unlimited ammo it displayes how many guesses
	{
		std::cout << "You have " << _guesses << " guesses left" << std::endl;
	}
	else//if unlimited ammo it displayes unlimited
	{
		std::cout << "You have Unlimited guesses left" << std::endl;
	}

}

void instructions(void)
{
	system("cls");
	std::cout << "How To Play BattleShips" << std::endl;
	std::cout << "********************************************************************************************************" << std::endl;
	std::cout << "The goal of battleships is to destroy all of the randomly generated ships with the limited ammunition." << std::endl;
	std::cout << "The computer will randomly generate 5 different type of ships ranging from 1 to 5 spaces long." << std::endl;
	std::cout << "The ships can be either horizontal or vertical." << std::endl;
	std::cout << "You can fire shots on the grid either hitting or missing a shot indicated by an X for hit and O for miss." << std::endl;
	std::cout << "To sink a ship you must destroy each indurvidual segment of the ship." << std::endl;
	std::cout << "You start the game with 40 ammunition with shooting spaces you already shot not removing ammo." << std::endl;
	std::cout << "The game ends when you either run out of shots or sink every ship with shots remaining." << std::endl;
	std::cout << "********************************************************************************************************" << std::endl;
	std::cout << "Press Enter To Return To Menu." << std::endl;
	std::cin.get();//waits for user input 
	std::cin.get();
}

void debugMode(bool& _debugMap, bool& _debugAmmo)
{
	bool exitFlag = false;
	int choice = 0;
	while (exitFlag == false)
	{
		system("cls");
		std::cout << "This is the debug menu where debug options can be toggled on or off. After, return to menu and launch the game." << std::endl;
		std::cout << "1. Toggle Debug Map" << std::endl;
		std::cout << "2. Toggle Unlimited Ammo" << std::endl;
		std::cout << "3. Return To Menu" << std::endl;
		std::cin >> choice;
		switch (choice)
		{
		case 1: _debugMap = !_debugMap;//if the debug map is on it is turned off it is off it is turned off
			if (_debugMap == true)
			{
				std::cout << "Debug Map turned on.";//prints out relative message based on what the user toggles
			}
			else
			{
				std::cout << "Debug Map turned off.";
			}
			Sleep(1500);
			break;
		case 2: _debugAmmo = !_debugAmmo;//unlimited ammo is turned off becomes on, on becomes off
			if (_debugAmmo == true)
			{
				std::cout << "Unlimited Ammo turned on.";
			}
			else
			{
				std::cout << "Unlimited Ammo turned off.";
			}
			Sleep(1500);
			break;
		case 3: exitFlag = true;
			break;
		default: std::cout << std::endl << "Invalid Option please try again";
			std::cin.clear();
			std::cin.ignore(100, '\n');
			Sleep(1500);
			break;
		}
	}
}