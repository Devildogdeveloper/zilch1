// Player.cpp


#include <string>
#include "Player.h"

using namespace std;
Player::Player()
{
}

// New player constructor
Player::Player(string n) : name(n), score(0), inGame(false)
{
}

// Show name
string Player::Name()
{
	return name;
}

// Add to Score
void Player::AddScore(int value)
{
	score += value;
}

// Show score
int Player::Score()
{
	return score;
}

// set inGame
void Player::SetInGame()
{
	inGame = true;
}

// return inGame status
bool Player::InGame()
{
	return inGame;
}