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

using namespace std;

//Useful structures

//The equivalent of real-world Scrabble letter tile pieces
typedef struct letterTile {
	char letter;
	int value;
} letterTile;

//For positioning tiles on the board
typedef struct coordinate {
	int x;
	int y;
} coordinate;

typedef struct proposedWord {
	string word;
	//To know where a word is to be placed, the only info needed is the starting coordinate and the direction
	//For the direction, 'u' = up, 'd' = down, 'l' = left, 'r' = right
	coordinate start;
	char direction;
} proposedWord;

//Prints a set of scrabble tiles
void printHand(vector<letterTile> hand);

//Generate the pool of letters to be played with in accordance with the English Scrabble letter frequency
vector<letterTile> generateLetterPool(int numberOfPlayers);

//Draw several letters from the pool and add them to a player's hand
vector<letterTile> drawLettersAndAddToHand(int numberOfLetters, vector<letterTile>* hand, vector<letterTile>* pool);

//Gets a letter from the pool and removes it from the pool
letterTile drawLetterChar(vector<letterTile> * pool);

//Checks if a word exists in a given dictionary, returns true if it does.  False otherwise.
bool isInDictionary(string s, string dictionary);

//Returns the point value of a letter in accordance with English Scrabble letter points
int getLetterValue(char c);

//Checks if a word can be formed using the existing letters on the board and the player’s tiles
bool canFormWord(proposedWord w, char* board, vector<letterTile> playerHand);

//Returns the value of placing a word at a certain position on the game board
int getWordValue(proposedWord w, char* board);

//Receives a pointer to the board matrix and a number of players
//Generates the board based on the player number and assigns values to the board variable
void generateBoard(char * board, int players);


/*
References
For creating dictionaries: http://app.aspell.net/create
Scrabble letter distribution: https://en.wikipedia.org/wiki/Scrabble_letter_distributions#English
Standard Scrabble board: https://i.stack.imgur.com/0kZO8.jpg
*/