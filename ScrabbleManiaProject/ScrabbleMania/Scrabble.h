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
		bool isFirstGameTurn;
		std::set<string> dictionary;
		board_t *board;
		vector<letterTile_t> letterPool;
		vector<proposedWord_t> addedWords;
		vector<Player> players;
		vector<int> scoreBoard;
		gameSettings_t gameSettings;

		// Variable for knowing whose turn it is
		int turn;

		//Generates the board based on the player number and assigns values to the board variable
		void generateBoard();

		// Print horizontal board separator
		void printBoardSeparator();

		// Fill dictionary with the settings from the user
		void fillDictionary();

		// Compares two players according to score
		// bool comparePlayerScore(Player i1, Player i2);

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
		bool hasActiveGame;

		// Constructor
		Scrabble();

		// New player wants to connect to game
		int addPlayerToGame(string name);

		// Set dictionary wanted by user
		void setDictionary(string dictionaryFileName);

		// Start the game
		void startGame();

		// End the game
		void endGame();

		/*** Actions in Game ***/

		// Get the name of the player
		string getPlayerName(int playerId);

		// Check if player is still in playing
		bool isPlayerStillInGame(int playerId);

		// Player is leaving game, return his position and score
		string playerIsLeaving(int playerId);

		// Player position depending on score
		int playerPosition(int playerId);

		// Get calculate scoreboard
		void updateScoreboard();

		// Get the number of active players
		int countActivePlayers();

		// Get the number of players
		int getPlayerNumber();

		//Returns a set of letters in scrabble tiles, separated by commas
		string getHand(int playerId);

		//Prints a set of scrabble tiles
		void printHand(int playerId);

		//Generate the pool of letters to be played with in accordance with the English Scrabble letter frequency
		void generateLetterPool();

		// Exchange specific letters from hand for random letters of letter pool
		void exchangeLetters(int playerId, vector<int> indexes);

		//Draw several letters from the pool and add them to a player's hand
		void drawLettersAndAddToHand(int numberOfLetters, int playerId);

		//Gets a letter from the pool and removes it from the pool
		letterTile_t drawLetterChar();

		//Returns the point value of a letter in accordance with English Scrabble letter points
		int getLetterValue(char c);

		// Adds word to board if it's a valid one
		int addWordToGame(proposedWord_t proposedWord, int playerId);

		//Returns the value of placing a word at a certain position on the game board
		int getWordValue(proposedWord_t proposedWord);

		//Get the char value from the board at a certain position
		char getBoardPosValue(int x, int y);

		//Set the board value at a position
		void setBoardPosValue(int x, int y, char value);

		//Print board
		void printBoard();

		// Get board size
		int getBoardSize();

		//Print all players' scores
		void printScores();

		//Set the number of players
		void setSettingsPlayerNumber(int pn);

		//Get the number of players
		int getSettingsPlayerNumber();

		// Get turn
		int getTurn();

		// Next turn
		void nextTurn();

		//Remove tile from a player's hand
		void removeTilesFromHand(int playerId, vector<char> neededLetters);

		// Get letter pool size
		int getLetterPoolSize();
};


/*
References
For creating dictionaries: http://app.aspell.net/create
Scrabble letter distribution: https://en.wikipedia.org/wiki/Scrabble_letter_distributions#English
Standard Scrabble board: https://i.stack.imgur.com/0kZO8.jpg
*/
