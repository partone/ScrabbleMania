/*
Luis Barajas
Eric Parton
*/

#include <string>
#include <string.h>
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

class ClientScrabble {
	private:
    board_t *board;

    // Save players size
    int playersNumber;

		// Print horizontal board separator
		void printBoardSeparator();

    //Set the board value at a position
    void setBoardPosValue(int x, int y, char value);
  public:
    // Constructor
		ClientScrabble();

    // set and get playersNumber
    void setPlayersNumber(int _playersNumber);
    int getPlayersNumber();

    //Generates the board based on the player number and assigns values to the board variable
		void generateBoard();

    //Returns the point value of a letter in accordance with English Scrabble letter points
		int getLetterValue(char c);

    // Adds word to board
		void addWordToGame(proposedWord_t proposedWord);

    //Print board
		void printBoard();

    // Get board size
		int getBoardSize();

    //Free board memory
		void freeBoard();
};
