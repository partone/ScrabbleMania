/*
Luis Barajas
Eric Parton
*/

#include "Scrabble.h"

//Generates a random letter in accordance with the English Scrabble letter frequency
char drawLetterChar();

//Checks if a word exists in a given dictionary, returns true if it does.  False otherwise.
bool isInDictionary(string s, string dictionary);

//Returns the point value of a letter in accordance with English Scrabble letter points
int getLetterValue(char c);

//Checks if a word can be formed using the existing letters on the board and the player’s tiles
int canFormWord(proposedWord w, char* board, vector<letterTile> playerHand);

//Returns the value of placing a word at a certain position on the game board
int getWordValue(proposedWord w, char* board);

//Receives a pointer to the board matrix and a number of players
//Generates the board based on the player number and assigns values to the board variable
void generateBoard(char* board, int players);