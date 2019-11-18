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
		board_t *board;
		vector<letterTile_t> letterPool;
		
		gameSettings_t gameSettings;

		//Generates the board based on the player number and assigns values to the board variable
		void generateBoard();
		// Fill dictionary with the settings from the user
		void fillDictionary();

		/* 
		* Checks if a word can be formed:
		* 1. Exists in the dictionary (isInDictionary function)
		* 2. Fits in the board considering the existing letters on it (canFormWord function)
		* 3. The player's tiles (canFormWord function)
		* 
		*/ 
		bool isValidWord(proposedWord_t proposedWord, int playerId);

		//Checks if a word exists in a given dictionary, returns true if it does.  False otherwise.
		bool isInDictionary(string s);

		//Checks if a word can be formed using the existing letters on the board and the player�s tiles
		bool canFormWord(proposedWord_t proposedWord, int playerId);

		// Check if words fits in board
		// Check if there are letters already in position, that they are equal to the index in word
		bool wordFitsInBoard(proposedWord_t proposedWord, vector<char> *neededLetters);

		//Write word onto board
		void placeLettersOnBoard(proposedWord_t proposedWord);

		//Get the vector of values on the existing board where the word will be placed
		vector<char> getBoardWordPath(proposedWord_t proposedWord);

		//Returns the value of a word, taking in to account bonuses
		int getWordValueWithBonuses(string word, vector<char> boardWordPath);

		//Free board memory
		void freeBoard();
	public:
		vector<Player> players;		//TODO: Move back to private
		// Constructor
		Scrabble();

		// New player wants to connect to game
		int addPlayerToGame(string name);

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

		// Exchange specific letters from hand for random letters of letter pool
		void exchangeLetters(int playerId, int indexes[], size_t numberOfIndexes);

		//Draw several letters from the pool and add them to a player's hand
		void drawLettersAndAddToHand(int numberOfLetters, int playerId);

		//Gets a letter from the pool and removes it from the pool
		letterTile_t drawLetterChar();

		//Returns the point value of a letter in accordance with English Scrabble letter points
		int getLetterValue(char c);

		// Adds word to board if it's a valid one
		bool addWordToGame(proposedWord_t proposedWord, int playerId);

		//Returns the value of placing a word at a certain position on the game board
		int getWordValue(proposedWord_t proposedWord);

		//Get the char value from the board at a certain position
		char getBoardPosValue(int x, int y);

		//Set the board value at a position
		void setBoardPosValue(int x, int y, char value);

		//Print board
		void printBoard();
};

/*
References
For creating dictionaries: http://app.aspell.net/create
Scrabble letter distribution: https://en.wikipedia.org/wiki/Scrabble_letter_distributions#English
Standard Scrabble board: https://i.stack.imgur.com/0kZO8.jpg
*/
