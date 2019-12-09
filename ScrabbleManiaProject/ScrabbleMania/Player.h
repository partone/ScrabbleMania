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
	bool stillInGame;
	Player(int _id);
	Player(int _id, string _name = "", player_type_t _playerType = ACTIVE);

	int getId();

	void setName(string _name);
	string getName();

	void setScore(int _score);
	int getScore();

	void setPlayerType(player_type_t _playerType);
	player_type_t getPlayerType();

	void addTileToHand(letterTile_t tile);
	void removeTileFromHand(char letter);

	vector<letterTile_t> * getHand();
	void setHand(vector<letterTile_t> newHand);		//For testing only

	//Prints a set of scrabble tiles
	void printHand();

	bool hasNeededLetters(vector<char> neededLetters);
	void printScore();
};

#endif  /* NOT PLAYER_H */
