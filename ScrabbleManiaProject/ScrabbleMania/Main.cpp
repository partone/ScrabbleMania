/*
Luis Barajas
Eric Parton
*/

#include "Scrabble.h"
#include "Player.h"

#define HAND_SIZE 7

int main() {

	Scrabble scrabble = Scrabble();

	//Generate a pool of letters to draw from
	/*int playerNumber = 4;
	vector<letterTile_t> letterPool = generateLetterPool(playerNumber);

	//Check pool size
	cout << "There are " << letterPool.size() << " letters in the letter pool" << endl;

	//Fill a player's hand
	vector<letterTile_t> playerHand1;
	playerHand1 = drawLettersAndAddToHand(HAND_SIZE, &playerHand1, &letterPool);
	cout << "Requesting 7 new tiles for the player" << endl;
	
	//Print the hand
	printHand(playerHand1);

	//Check pool size
	cout << "There are now " << letterPool.size() << " letters in the pool" << endl;

	//Generate a board
	generateBoard(playerNumber);
	cout << board[0][0] << endl;
	//printBoard(playerNumber);
	freeBoard(board, playerNumber);

	cin >> playerNumber;*/

	scrabble.addPlayerToGame("Luis");

	scrabble.setSettings("dictionaries/english.txt");

	scrabble.startGame();
	
	proposedWord_t proposedWord = proposedWord_t("bee", 0, 0, 'u');

	cout << scrabble.isValidWord(proposedWord, 0) << endl;

	scrabble.endGame();
	
	return 1;
}
