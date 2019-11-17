/*
Luis Barajas
Eric Parton
*/

#include "Scrabble.h"

// Constructor
Scrabble::Scrabble(){
	hasGameStarted = false;
	board = NULL;
}

// New player wants to connect to game
void Scrabble::addPlayerToGame(string name){
	if (!hasGameStarted){
		// The id will be its index at players' vector
		Player newPlayer = Player(players.size(), name);
		players.push_back(newPlayer);
	}else{
		// Do not add it but let it expectate
	}
}

// Set settings wanted by user
void Scrabble::setSettings(string dictionaryFileName){
	gameSettings.dictionaryFileName = dictionaryFileName;
}

// Start the game
void Scrabble::startGame(){
	if(!hasGameStarted) {
		hasGameStarted = true;
		fillDictionary();
		generateBoard();
		printBoard();
	}
}

// End the game
void Scrabble::endGame(){
	if(hasGameStarted) {
		hasGameStarted = false;

		freeBoard();

		// TODO: calculate winner, etc...

	}
}

//Prints a set of scrabble tiles
void Scrabble::printHand(int playerId) {
	vector<letterTile_t> *hand = players[playerId].getHand();
	for (int i = 0; i < hand->size(); i++) {
		cout << hand->at(i).letter << " " << hand->at(i).value;
		if (i != hand->size() - 1) {
			cout << " // ";
		}
	}
	cout << endl;
}

// Fill dictionary with the settings from the user
void Scrabble::fillDictionary(){
	string line;
  ifstream file (gameSettings.dictionaryFileName);

  if (file.is_open()) {
    while ( getline (file,line) ) {
      // Save it in a the dictionary data structure
			dictionary.insert(line);
    }
    file.close();
  }

  else cout << "Unable to open file"; 
}

//Generate the pool of letters to be played with in accordance with the English Scrabble letter frequency
void Scrabble::generateLetterPool() {
	char letterList[] = { ' ', ' ',
						'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E',
						'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
						'I', 'I', 'I', 'I', 'I', 'I', 'I', 'I', 'I',
						'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 
						'N', 'N', 'N', 'N', 'N', 'N', 
						'R', 'R', 'R', 'R', 'R', 'R',
						'T', 'T', 'T', 'T', 'T', 'T', 
						'L', 'L', 'L', 'L', 
						'S', 'S', 'S', 'S', 
						'U', 'U', 'U', 'U', 
						'D', 'D', 'D', 'D', 
						'G', 'G', 'G', 
						'B', 'B',
						'C', 'C', 
						'M', 'M',
						'P', 'P',
						'F', 'F',
						'H', 'H',
						'V', 'V',
						'W', 'W',
						'Y', 'Y',
						'K', 'J', 'X', 'Q', 'Z'};
	int cycler = 0;
	int numberOfTiles = players.size() * 25;
	//Calculate a full set of letters for each player, then shuffle, then prune 3/4 of them
	//That way the letters are evenly distributed
	int numberOfCycles = players.size() * 100;


	//Go around the letter list adding letters until enough tiles have been pushed
	for (int i = 0; i < numberOfCycles; i++) {
		letterTile_t tmp;								//Create a generic letter tile 
		tmp.letter = letterList[cycler];			//Assign it a letter
		tmp.value = getLetterValue(tmp.letter);		//Get the letter's value
		letterPool.push_back(tmp);					//Push the new letter in

		cycler++;
		//If it reaches the end of the list, go back to the start
		if(cycler >= sizeof(letterList)/sizeof(*letterList))
			cycler = 0;
	}

	//Shuffle the pool
	random_shuffle(letterPool.begin(), letterPool.end());

	//Prune the extra tiles
	letterPool.resize(numberOfTiles);
}

//Draw several letters from the pool and add them to a player's hand
void Scrabble::drawLettersAndAddToHand(int numberOfLetters, int playerId) {
	//Push fresh pool values in to the hand
	for (int i = 0; i < numberOfLetters; i++) {
		players[playerId].addTileToHand(drawLetterChar());
	}
}

//Gets a letter from the pool and removes it from the pool 
letterTile_t Scrabble::drawLetterChar() {
	letterTile_t returnVal = letterPool.back();		//Get the last element
	letterPool.pop_back();					//Pop the last element
	return returnVal;					//Return the letter
}

// Checks if a word exists in a given dictionary, returns true if it does.  False otherwise.
bool Scrabble::isInDictionary(string s) {
	return dictionary.find(s) != dictionary.end();
}

//Returns the point value of a letter in accordance with English Scrabble letter points
int Scrabble::getLetterValue(char c) {
	switch (c) {
	case 'A': case 'E': case 'I': case 'O': case 'U': case 'L': case 'N': case 'S': case 'T': case 'R':
		return 1;
	case 'D': case 'G':
		return 2;
	case 'B': case 'C': case 'M': case 'P':
		return 3;
	case 'F': case 'H': case 'V': case 'W': case 'Y':
		return 4;
	case 'K':
		return 5;
	case 'J': case 'X':
		return 8;
	case 'Q': case 'Z':
		return 10;
	case ' ':
		return 0;
	default:
		cout  << "Error, invalid character" << endl;
		return -1;
	}
}

/* 
 * Checks if a word can be formed:
 * 1. Exists in the dictionary (isInDictionary function)
 * 2. TODO: Fits in the board considering the existing letters on it (canFormWord function)
 * 3. TODO: The player's tiles (canFormWord function)
 * 
 */ 
bool Scrabble::isValidWord(proposedWord_t proposedWord, int playerId) {
	return isInDictionary(proposedWord.word) && canFormWord(proposedWord, playerId);
}

// TODO: Checks if a word can be formed using the existing letters on the board and the player's tiles
bool Scrabble::canFormWord(proposedWord_t proposedWord, int playerId) {
	return true;
}

//TODO: Returns the value of placing a word at a certain position on the game board
int Scrabble::getWordValue(proposedWord_t proposedWord) {
	return 1;
}

//Generates the board based on the player number and assigns values to the board variable
void Scrabble::generateBoard() {
	int boardSize = 15;						//Standard board size for 4 players

	board = (board_t *)malloc(sizeof(board_t));
  board->data = NULL;
	board->size = players.size() > 4 ? boardSize / 4 * players.size() : boardSize;	//Adjust it for our player number if it is greater than 4
	//If the board size comes out even, increase it by one

	if (board->size % 2 == 0)
		board->size++;

	//Allocate memory to the board
	board->data = (char**)malloc(board->size * sizeof(char*));			//Cast necessary because of the C++ compiler
	for (int i = 0; i < board->size; i++) {
		board->data[i] = (char*)malloc(board->size * sizeof(char));
	}

	//Assign tile roles
	//0 = Regular tile
	//1 = Double letter score
	//2 = Triple letter score
	//3 = Double word score
	//4 = Triple word score
	//Any other char = Placed letter(ex.A, B, C, etc.)


	//Set all positions of the board to be regular tiles
	for (int i = 0; i < board->size; i++) {
		for (int j = 0; j < board->size; j++) {
			if ((i == j) || (i == ((board->size - 1) - j))) {
				//Double word
				board->data[i][j] = '3';
			}
			else {
				//Regular tile
				board->data[i][j] = '0';
			}
		}
	}

	//Central multipliers
	board->data[board->size / 2 + 1][board->size / 2 + 1] = '1';
	board->data[board->size / 2 + 1][board->size / 2 - 1] = '1';
	board->data[board->size / 2 - 1][board->size / 2 + 1] = '1';
	board->data[board->size / 2 - 1][board->size / 2 - 1] = '1';

	board->data[board->size / 2 + 2][board->size / 2 + 2] = '2';
	board->data[board->size / 2 + 2][board->size / 2 - 2] = '2';
	board->data[board->size / 2 - 2][board->size / 2 + 2] = '2';
	board->data[board->size / 2 - 2][board->size / 2 - 2] = '2';

	//Triple word corners
	board->data[0][0] = '4';
	board->data[0][board->size - 1] = '4';
	board->data[board->size - 1][0] = '4';
	board->data[board->size - 1][board->size - 1] = '4';

	//Triple word centres
	board->data[0][board->size / 2] = '4';
	board->data[board->size - 1][board->size / 2] = '4';
	board->data[board->size / 2][board->size - 1] = '4';
	board->data[board->size / 2][0] = '4';

}

//Print board
void Scrabble::printBoard() {
  for(int i = 0; i < board->size; i++){
    for(int j = 0; j < board->size; j++){
      printf("%c ", board->data[i][j]);
    }
    printf("\n");
  }
}

//Free board memory
void Scrabble::freeBoard() {
	for(int i = 0; i < board->size; i++){
    free(board->data[i]);
  }
  free(board->data);
  free(board);
}
