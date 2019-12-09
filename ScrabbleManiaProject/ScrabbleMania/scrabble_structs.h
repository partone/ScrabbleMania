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

// Board struct
typedef struct board_struct {
	// Save the state of the words' board
  char **data;
  // save size of board
  int size;
} board_t;

//The equivalent of real-world Scrabble letter tile pieces
typedef struct letter_tile_struct {
	char letter;
	int value;
	letter_tile_struct(){
		letter = 'a';
		value = 0;
	}
	letter_tile_struct(char _letter, int _value){
		letter = _letter;
		value = _value;
	}
} letterTile_t;

//For positioning tiles on the board
typedef struct coordinate_struct {
	int x;
	int y;
	coordinate_struct(){
		x = 0;
		y = 0;
	}
	coordinate_struct(int _x, int _y){
		x = _x;
		y = _y;
	}
} coordinate_t;

typedef struct proposed_word_struct {
	string word;
	//To know where a word is to be placed, the only info needed is the starting coordinate_t and the direction
	//For the direction, 'u' = up, 'd' = down, 'l' = left, 'r' = right
	coordinate_t start;
	char direction;
	proposed_word_struct(){
		word = "";
		start = coordinate_t();
		direction = 'u';
	}
	proposed_word_struct(string _word, int _y, int _x, char _direction){
		// Change word to uppercase
		std::transform(_word.begin(), _word.end(),_word.begin(), ::toupper);
		word = _word;
		start = coordinate_t(_x, _y);
		direction = _direction;
	}
} proposedWord_t;

// Game seetings in each game
typedef struct game_settings_struct {
	string dictionaryFileName;
	int playerNumber;

	game_settings_struct(){
		dictionaryFileName = "";
	}
	game_settings_struct(string _dictionaryFileName){
		dictionaryFileName = _dictionaryFileName;
	}
} gameSettings_t;

// Player types
typedef enum player_type_enum {ACTIVE, SPECTATOR} player_type_t;

#endif  /* NOT SCRABBLE_STRUCTS_H */
