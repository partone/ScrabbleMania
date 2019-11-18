/*
Luis Barajas
Eric Parton
*/

#include "Scrabble.h"
#include "Player.h"

#define HAND_SIZE 7

int main() {

	Scrabble scrabble = Scrabble();

	int idLuis = scrabble.addPlayerToGame("Luis");
	int idEric = scrabble.addPlayerToGame("Eric");

	scrabble.setSettings("dictionaries/english.txt");

	scrabble.startGame();

	letterTile_t a = letterTile_t('O', 1);
	letterTile_t b = letterTile_t('L', 1);
	letterTile_t c = letterTile_t('H', 1);
	letterTile_t d = letterTile_t('L', 1);
	letterTile_t e = letterTile_t('E', 1);
	letterTile_t f = letterTile_t('W', 1);
	letterTile_t g = letterTile_t('F', 1);
	vector<letterTile_t> fakeHand = { a, b, c, d, e, f, g };
	
	scrabble.players[0].setHand(fakeHand);
	scrabble.printHand(idLuis);
	
	proposedWord_t proposedWord = proposedWord_t("hello", 7, 7, 'r');

	cout << scrabble.addWordToGame(proposedWord, idLuis) << endl;

	scrabble.printBoard();

	scrabble.endGame();

	cin >> idEric;
	
	return 1;
}
