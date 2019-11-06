/*
Luis Barajas
Eric Parton
*/

#include "Scrabble.h"

#define HAND_SIZE 7

int main() {


	//Generate a pool of letters to draw from
	int playerNumber = 4;
	vector<letterTile> letterPool = generateLetterPool(playerNumber);

	//Check pool size
	cout << "There are " << letterPool.size() << " letters in the letter pool" << endl;

	//Fill a player's hand
	vector<letterTile> playerHand1;
	playerHand1 = drawLettersAndAddToHand(HAND_SIZE, &playerHand1, &letterPool);
	cout << "Requesting 7 new tiles for the player" << endl;
	
	//Print the hand
	printHand(playerHand1);

	//Check pool size
	cout << "There are now " << letterPool.size() << " letters in the pool" << endl;

	//Generate a board
	char** board = NULL;
	generateBoard(board, playerNumber);
	cout << board[0][0] << endl;
	//printBoard(board, playerNumber);
	freeBoard(board, playerNumber);

	cin >> playerNumber;
	return 1;
}