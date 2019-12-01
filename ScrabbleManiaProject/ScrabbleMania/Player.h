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
public:
	Player(int _id);
	Player(int _id, string _name = "");

	int getId();

	void setName(string _name);
	string getName();
	
	void setScore(int _score);
	int getScore();

	void addTileToHand(letterTile_t tile);
	vector<letterTile_t> * getHand();
	void setHand(vector<letterTile_t> newHand);		//For testing only

	bool hasNeededLetters(vector<char> neededLetters);
	void printScore();
};

#endif  /* NOT PLAYER_H */

