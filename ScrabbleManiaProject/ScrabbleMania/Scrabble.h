/*
Luis Barajas
Eric Parton
*/

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
#include "Player.h"

using namespace std;

class Scrabble {
	private:
		bool hasGameStarted;
		std::set<string> dictionary;
		char** board;
		vector<letterTile_t> letterPool;
		vector<Player> players;
		gameSettings_t gameSettings;

		//Generates the board based on the player number and assigns values to the board variable
		void generateBoard();
		// Fill dictionary with the settings from the user
		void fillDictionary();

		//Checks if a word exists in a given dictionary, returns true if it does.  False otherwise.
		bool isInDictionary(string s);
		//Checks if a word can be formed using the existing letters on the board and the player�s tiles
		bool canFormWord(proposedWord_t proposedWord, int playerId);

		//Free board memory
		void freeBoard();
	public:
		// Constructor
		Scrabble();

		// New player wants to connect to game
		void addPlayerToGame(string name);

		// Set settings wanted by user
		void setSettings(string dictionaryFileName);

		// Start the game
		void startGame();

		// End the game
		void endGame();

		/*** Actions in Game ***/

		//Prints a set of scrabble tiles
		void printHand(int playerId);

		//Generate the pool of letters to be played with in accordance with the English Scrabble letter frequency
		void generateLetterPool();

		//Draw several letters from the pool and add them to a player's hand
		void drawLettersAndAddToHand(int numberOfLetters, int playerId);

		//Gets a letter from the pool and removes it from the pool
		letterTile_t drawLetterChar();

		//Returns the point value of a letter in accordance with English Scrabble letter points
		int getLetterValue(char c);

		/* 
		* Checks if a word can be formed:
		* 1. Exists in the dictionary (isInDictionary function)
		* 2. Fits in the board considering the existing letters on it (canFormWord function)
		* 3. The player's tiles (canFormWord function)
		* 
		*/ 
		bool isValidWord(proposedWord_t proposedWord, int playerId);

		//Returns the value of placing a word at a certain position on the game board
		int getWordValue(proposedWord_t proposedWord);

		//Print board
		void printBoard();
};

/*
References
For creating dictionaries: http://app.aspell.net/create
Scrabble letter distribution: https://en.wikipedia.org/wiki/Scrabble_letter_distributions#English
Standard Scrabble board: https://i.stack.imgur.com/0kZO8.jpg
*/
