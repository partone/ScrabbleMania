/*
Luis Barajas
Eric Parton
*/

#include "Scrabble.h"
#include "Player.h"

#define HAND_SIZE 7

// Function declarations
void playersLobby(Scrabble *scrabble);
void game(Scrabble *scrabble);

int main() {

	Scrabble scrabble = Scrabble();

	playersLobby(&scrabble);

	game(&scrabble);
	
	return 1;
}

/*
	Function for the players lobby before the game
*/
void playersLobby(Scrabble *scrabble){
	scrabble->addPlayerToGame("Luis");
	scrabble->addPlayerToGame("Eric");

	scrabble->setSettings("dictionaries/english.txt");
}

/*
	Function where the game is executed
*/
void game(Scrabble *scrabble) {
	scrabble->startGame();
	int i = 0;
	while(scrabble->hasActiveGame){
		scrabble->printBoard();
		if (i == 10) {
			scrabble->endGame();
		}
		i++;
	}
	
	// scrabble->printBoard();
	// scrabble->endGame();
}
