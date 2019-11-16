/*
Luis Barajas
Eric Parton
*/

#ifndef SCRABBLE_STRUCTS_H
#define SCRABBLE_STRUCTS_H

#include <string>
#include <iostream>

using namespace std;

//Useful structures

//The equivalent of real-world Scrabble letter tile pieces
typedef struct letterTileStruct {
	char letter;
	int value;
	letterTileStruct(){
		letter = 'a';
		value = 0;
	}
	letterTileStruct(char _letter, int _value){
		letter = _letter;
		value = _value;
	}
} letterTile_t;

//For positioning tiles on the board
typedef struct coordinateStruct {
	int x;
	int y;
	coordinateStruct(){
		x = 0;
		y = 0;
	}
	coordinateStruct(int _x, int _y){
		x = _x;
		y = _y;
	}
} coordinate_t;

typedef struct proposedWordStruct {
	string word;
	//To know where a word is to be placed, the only info needed is the starting coordinate_t and the direction
	//For the direction, 'u' = up, 'd' = down, 'l' = left, 'r' = right
	coordinate_t start;
	char direction;
	proposedWordStruct(){
		word = "";
		start = coordinate_t();
		direction = 'u';
	}
	proposedWordStruct(string _word, int _x, int _y, char _direction){
		word = _word;
		start = coordinate_t(_x, _y);
		direction = _direction;
	}
} proposedWord_t;

// Game seetings in each game
typedef struct gameSettingsStruct {
	string dictionaryFileName;
	// TODO: add settings that the user can set

	gameSettingsStruct(){
		dictionaryFileName = "";
	}
	gameSettingsStruct(string _dictionaryFileName){
		dictionaryFileName = _dictionaryFileName;
	}
} gameSettings_t;

#endif  /* NOT SCRABBLE_STRUCTS_H */
