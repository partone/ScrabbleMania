/*
Luis Barajas
Eric Parton
*/

#include "Scrabble.h"

#define HAND_SIZE 7

int main() {
	int playerNumber = 2;
	vector<letterTile> letterPool = generateLetterPool(playerNumber);

	cout << "There are " << letterPool.size() << " letters in the letter pool" << endl;

	vector<letterTile> playerHand1;
	playerHand1 = drawLettersAndAddToHand(HAND_SIZE, &playerHand1, &letterPool);
	cout << "Requesting 7 new tiles for the player" << endl;
	
	printHand(playerHand1);

	cout << "There are now " << letterPool.size() << " letters in the pool" << endl;

	cin >> playerNumber;
	return 1;
}