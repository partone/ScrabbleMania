/*
Luis Barajas
Eric Parton
*/

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include "scrabble_structs.h"

using namespace std;

class Player {
private:
	int id;
	string name;
	vector<letterTile_t> hand;
	int score;
	player_type_t playerType;
public:
	// Variable to validate if player has not left game yet 
	bool stillInGame;
	// Constructors
	Player(int _id);
	Player(int _id, string _name = "", player_type_t _playerType = ACTIVE);
	// Get the id of the player
	int getId();

	// Get and set name of the player
	void setName(string _name);
	string getName();

	// Get and set score of the player
	void setScore(int _score);
	int getScore();

	// Get and set type of the player
	void setPlayerType(player_type_t _playerType);
	player_type_t getPlayerType();

	// Add specific tile to player's hand
	void addTileToHand(letterTile_t tile);
	// Remove specific tile from player's hand
	void removeTileFromHand(char letter);

	// Get reference to player's hand
	vector<letterTile_t> * getHand();
	// Set hand of the player (testing only)
	void setHand(vector<letterTile_t> newHand);	

	//Prints a set of scrabble tiles
	void printHand();

	// Check if player has the needed letters from vector
	bool hasNeededLetters(vector<char> neededLetters);
	// Print current player's score
	void printScore();
};

#endif  /* NOT PLAYER_H */
