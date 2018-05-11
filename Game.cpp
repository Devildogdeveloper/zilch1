
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <array>

#include "Player.h"
#include "Game.h"
#include "MenuOptions.h"

using namespace std;

#pragma region Inputs
// Clears the input stream when invalid characters are used
void Game::ClearInput()
{
	// if an invalid number is detected, clear the input stream
	cin.clear();

	// remove characters from the input stream until a newline or EOF is met			
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Returns an Integer for player input
int Game::GetInput()
{
	unsigned int sel;

	// keep repeating until a valid number is stored in sel
	while (!(cin >> sel))
	{
		ClearInput();

		cout << "Invalid selection. Try again: ";
	}

	ClearInput();

	return sel;
}
#pragma endregion

#pragma region Messages
// Congratulate the Champion
void Game::CongChamp(Player& player)
{
	system("cls");
	
	cout << "Hey Hey Hey!" << endl;
	cout << "=================" << endl;
	cout << player.Name() << " just kicked your butt! " << "They got" << player.Score() << "points!" << endl;
	cout << "They were probably cheating though" << endl;

	system("pause");
}

// Alert the players the final round has begun
void Game::MatchPoints(Player& player)
{
	system("cls");
	cout << "**********************************************************************" << endl;
	cout << "*ITS THE FINAL ZILCH ROUNDDDDDDD (in the tune of terrible 80's music)*" << endl;
	cout << "**********************************************************************" << endl;
	cout << player.Name() << " is beating everyone with " << player.Score() << "!" << endl; 


	system("pause");
}

// Display dice kept this round
void Game::ShowKeep(array<int, 6>& keep)
{
	cout << "**************" << endl;
	cout << "* Kept Dice  *" << endl;
	cout << "**************" << endl;

	for (unsigned int i = 0; i < keep.size(); i++)
	{
		cout << "[" << (i + 1) << "]: " << keep[i];

		if (i != keep.size() - 1)
		{
			cout << ", ";
		}
	}

	cout << endl << endl;
}

// Display current dice pool values
void Game::ShowRolled(vector<int>& dicePool)
{
	cout << "**************" << endl;
	cout << "* Dice Rolls *" << endl;
	cout << "**************" << endl;

	for (unsigned int i = 0; i < dicePool.size(); i++)
	{
		cout << "| Die " << (i + 1) << ": [" << dicePool[i] << "] |" << endl;
	}

	cout << "**************" << endl << endl;
}

#pragma endregion

#pragma region Setup
// Prompt the user for the Number of Players
void Game::PromptNumPlayers()
{
	system("cls");
	
	cout << "NEW GAME" << endl;
	cout << "------------------------------" << endl;
	cout << "How many players are there? Please enter a number from 1-4: ";
}

// Setup player names
void Game::SetupPlayerNames(vector<Player>& players)
{
	system("cls");
	cout << "WHO WILL BE THE CHAMPION?" << endl;
	cout << "------------------------------" << endl;
	
	// Get the name for each player in the players vector
	for (unsigned int i = 0; i < players.size(); i++)
	{
		string name;
		
		cout << "What is your name Champion? " << (i + 1) << ": ";
		
		cin >> name;

		ClearInput();

		// create a new player object in the player vector using the name provided
		players[i] = Player(name);	
	}
}

#pragma endregion

#pragma region Play
// Counts different dice values in a roll
array<int, 6> Game::DiceCounter(vector<int>& dicePool)
{
	// Create new array and set all values to 0
	array<int, 6> counter = { 0 };

	// Count each die for it's value and add that to the counter array
	for (unsigned int i = 0; i < dicePool.size(); i++)
	{
		counter[dicePool[i] - 1]++;
	}

	return counter;
}

// Check if a player is winning limit
bool Game::CheckForWin(int score)
{
	bool isWinning = score >= 5000 ? true : false;

	return isWinning;
}

// Check if the dice roll has any scoring dice
bool Game::ScoringDice(array<int, 6>& diceCount)
{
	// If there is a 1 or a 5, return true
	if (diceCount[0] >= 1 || diceCount[4] >= 1)
	{
		return true;
	}
	
	// If no 1 or 5, check for any triples
	for (unsigned int i = 1; i < diceCount.size(); i++)
	{
		if (i == 4) {
			i = 5; 
		}

		if (diceCount[i] >= 3) { 
			return true; 
		}
	}

	
	// No scoring dice
	return false;
}

// Checks if the selection is a valid die in the dice pool
bool Game::IsValid(unsigned int& selection, vector<int>& dicePool)
{
	if (selection > 0 && selection <= dicePool.size())
	{
		return true;
	}

	return false;
}

// Scores and removes the dice selected by the player
int Game::Score(vector<int>& dicePool, int die, array<int, 6>& dieCount, array<int, 6>& keptDice)
{
	die -= 1;	
	int score = 0;
	int dieValue = dicePool[die];

	// If there are 3 or more dice with the selected value, set aside 3 of the dice
	if (dieCount[dieValue - 1] >= 3)
	{
		int i = 0;
		while (i < 3)
		{
			for (unsigned int k = 0; k < dicePool.size(); k++)
			{
				if (dicePool[k] == dieValue)
				{					
					dicePool.erase(dicePool.begin() + k); // Remove the selected die from the dicepool
					keptDice[dieValue - 1]++; // Increment the dice kept for this turn
					i++;
				}
			}
		}

		// if 3 1's were kept, the score is 1000, otherwise is 100x the die value
		score = dieValue == 1 ? 1000 : dieValue * 100;
	}
	else if (dieValue == 1 || dieValue == 5)
	{			
		dicePool.erase(dicePool.begin() + die);	// Remove selected die from the dicepool
		keptDice[dieValue - 1]++; // Incredment the dice kept for this turn

		// If die is 1, score is 100, otherwise it's 50
		score = dieValue == 1 ? 100 : 50;
	}
	//check for straight
	//check for three Pairs
	//check for full house (one triple & one pair)
	//check for 4 of a kind
	//check for 6 of a kind


	return score;
}

// A method to roll all the dice in a provided pool
void Game::DiceRoll(vector<int>& dicePool)
{
	random_device rng;

	for (unsigned int i = 0; i < dicePool.size(); i++)
	{
		dicePool[i] = rng() % 6 + 1; // Creates a random number from 1 to 6
	}
}

// Get the name of the player with the highest score
Player Game::GetChampion(vector<Player> players)
{
	Player Champion = players[0];

	for (unsigned int i = 1; i < players.size(); i++)
	{
		if (players[i].Score() > players[i - 1].Score())
		{
			Champion = players[i];
		}
	}

	return Champion;
}
#pragma endregion

#pragma region Private Loops
// Primary Game Loop
Player Game::Play()
{
	Player Champion; // While this string is empty, the game will continue to loop
	int numOfPlayers = 0; // Int to hold the number of players
	int playerTurn = 0; // Int that keeps track of which players turn it is, based on vector index
	int firstToWin = -1; // Int to mark the first players turn that has hit 10k
	int turnScore = 0; // The value the player has scored this turn

	PromptNumPlayers();

	// Loop until players is at least 2
	while (!(numOfPlayers > 1))
	{
		numOfPlayers = GetInput();

		if (numOfPlayers < 2)
		{
			cout << "Invalid number of players. Must be more than 1." << endl;
			cout << "Try again: ";
		}
	}

	vector<Player> players(numOfPlayers); // generate a vector to hold Player objects based on number of players

	SetupPlayerNames(players);

	//WelcomePlayers(players);

	// Continue looping until a Champion is declared
	while (Champion.Name() == "") 
	{
		// Return to first players turn if necessary
		if (playerTurn == players.size())
		{
			playerTurn = 0;
		}

		// The game is over if all players have had a turn after a player reaches 10k
		if (firstToWin >= 0 && playerTurn == firstToWin)
		{
			Champion = GetChampion(players);
			continue;
		}

		// Run a turn for a player and put their score into the turnScore variable
		turnScore = Turn(players[playerTurn]);

		// If the player has not entered the game and they score more than the meld points
		if (!players[playerTurn].InGame() && turnScore >= 1000)
		{
			players[playerTurn].SetInGame(); // put player in the game
		}

		// add turnscore to players score
		if (turnScore > 0)
		{
			players[playerTurn].AddScore(turnScore);
		}

		// Check if players score is over winning threshold
		if (firstToWin == -1 && CheckForWin(players[playerTurn].Score()))
		{
			MatchPoints(players[playerTurn]);

			firstToWin = playerTurn;
		}

		playerTurn++; // Send to next players turn
	}

	return Champion;
}

// Turn loop
int Game::Turn(Player& player)
{
	vector<int> dicePool(6); // New dice pool of 6 dice
	array<int, 6> keptDice = { 0 }; // Set all kept dice values to 0
	array<int, 6> diceCount = { 0 }; // Variable to count die values
	int turnScore = 0; // Running score for this turn
	bool zilched = false; // Determines if player has zilched
	bool canPass = player.InGame(); // Determines if player can pass their turn
	bool canReroll = false; // Determines if player can reroll the dice
	bool hasPassed = false; // Determines when the player has passed their turn

	unsigned int selection = 1;

	while (!hasPassed)
	{
		// if player has set aside all dice
		if (dicePool.size() == 0)
		{
			// populate dicePool back to 6 dice
			for (int i = 0; i < 6; i++)
			{
				dicePool.push_back(1);
			}
		}
		
		DiceRoll(dicePool);
		canReroll = false;
		canPass = false;

		// Allow player to set aside the rolled dice until they pass or reroll
		do
		{
			system("cls");
			cout << player.Name() << " is rolling" << endl;
			cout << player.Name() << "'s Score: " << player.Score() << endl;
			cout << "Turn Score: " << turnScore << endl << endl;

			ShowRolled(dicePool);
			ShowKeep(keptDice);

			diceCount = DiceCounter(dicePool);

			// If the player has no scoring dice and they can't reroll
			if (!ScoringDice(diceCount) && !canReroll)
			{				
				zilched = true; // The player has zilched
				selection = 0; // Automatically select to end their turn
				continue; 
			}	

			cout << "Enter the die # you wish to keep. Triple values will automatically be kept." << endl;
			cout << "Enter 9 to reroll dice or 0 to end your turn." << endl;
			cout << "Selection: ";

			// If the dice pool is empty, automatically reroll to generate 6 new dice, otherwise get the player's selection
			selection = dicePool.size() == 0 ? 9 : GetInput();

			if (IsValid(selection, dicePool))
			{
				// Add the dice value to turn score
				turnScore += Score(dicePool, selection, diceCount, keptDice);
							
				canReroll = true; // Player has kept a die and can now reroll the dice
				
				// If the player is in the game or they have over 1000 points, the player can end their turn
				canPass = (player.InGame() || turnScore >= 1000) ? true : false;

			}			
			// check if the player has selected a valid move, ending turn or rerolling dice
			else if ((selection == 0 && !canPass) || (selection == 9 && !canReroll))
			{
				string msg;

				system("cls");
				cout << "*****************" << endl;
				cout << "*     NOPE!     *" << endl;
				cout << "*****************" << endl;

				msg = selection == 0 ? player.Name() + ", you're not done yet! you gotta roll again!" : player.Name() + " is trying to break the rules!";

				cout << msg << endl << endl;

				selection = 7;

				system("pause");
			}
		} while (selection > 0 && selection != 9);

		// Player selected to end their turn, then they have passed. Otherwise they're rolling dice
		hasPassed = selection == 0 ? true : false;
	}

	system("cls");
	cout << "------------------" << endl;
	cout << player.Name() << "'s turn is over!" << endl;
	cout << "------------------" << endl;

	if (zilched)
	{
		cout << "You ain't got nothing but a zilch " << player.Name() << "!" << endl;
	}
	else
	{
		cout << "Hey " << player.Name() << "!" << " You got " << turnScore << " points from this turn" << "!" << endl;
	}

	system("pause");

	// return 0 if player zilched or turnScore
	return turnScore = zilched ? 0 : turnScore;
}
#pragma endregion

void Game::MainGameLoop()
{
	bool quit = false;
	unsigned int selection;
	Player Champion;

	while (!quit)
	{
		MenuOptions::ShowMenu();
		selection = GetInput();

		switch (selection)
		{
		case 1:	// New Game
			Champion = Play();
			CongChamp(Champion);
			break;
		case 2:	// Quit
			quit = true;
			break;
		default: // Anything else
			break;
		}		
	}	
}