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

	/*
	letterTile_t a = letterTile_t('O', 1);
	letterTile_t b = letterTile_t('L', 1);
	letterTile_t c = letterTile_t('H', 1);
	letterTile_t d = letterTile_t('L', 1);
	letterTile_t e = letterTile_t('E', 1);
	letterTile_t f = letterTile_t('W', 1);
	letterTile_t g = letterTile_t('F', 1);
	vector<letterTile_t> fakeHand = { a, b, c, d, e, f, g }; g++ gives an error here*/
	vector<letterTile_t> fakeHand;
	fakeHand.push_back(letterTile_t('O', 1));
	fakeHand.push_back(letterTile_t('L', 1));
	fakeHand.push_back(letterTile_t('H', 1));
	fakeHand.push_back(letterTile_t('L', 1));
	fakeHand.push_back(letterTile_t('E', 1));
	fakeHand.push_back(letterTile_t('W', 1));
	fakeHand.push_back(letterTile_t('F', 1));
	
	scrabble.players[0].setHand(fakeHand);
	scrabble.printHand(idLuis);
	
	proposedWord_t proposedWord = proposedWord_t("hello", 7, 7, 'r');

	scrabble.addWordToGame(proposedWord, idLuis);

	scrabble.printBoard();

	vector<letterTile_t> fakeHand2;
	fakeHand2.push_back(letterTile_t('O', 1));
	fakeHand2.push_back(letterTile_t('H', 1));
	fakeHand2.push_back(letterTile_t('L', 1));
	fakeHand2.push_back(letterTile_t('L', 1));
	fakeHand2.push_back(letterTile_t('E', 1));
	fakeHand2.push_back(letterTile_t('W', 1));
	fakeHand2.push_back(letterTile_t('F', 1));

	scrabble.players[0].setHand(fakeHand2);
	scrabble.printHand(idLuis);
	
	proposedWord = proposedWord_t("hello", 10, 4, 'd');

	scrabble.addWordToGame(proposedWord, idLuis);

	scrabble.printBoard();

	scrabble.endGame();

	cin >> idEric; // TODO: remove when submit
	
	return 1;
}
