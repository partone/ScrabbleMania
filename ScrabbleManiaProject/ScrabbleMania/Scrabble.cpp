/*
Luis Barajas
Eric Parton
*/

#include "Scrabble.h"

//Prints a set of scrabble tiles
void Scrabble::printHand(vector<letterTile> hand) {
	 for (int i = 0; i < hand.size(); i++) {
		 cout << hand[i].letter << " " << hand[i].value;
		 if (i != hand.size() - 1) {
			 cout << " // ";
		 }
	 }
	 cout << endl;
}

// Fill dictionary with the settings from the user
void Scrabble::fillDictionary(string dictionaryFileName){
	string line;
  ifstream file (dictionaryFileName);

  if (file.is_open()) {
    while ( getline (file,line) ) {
      // Save it in a the dictionary data structure
			dictionary[line] = true;
    }
    file.close();
  }

  else cout << "Unable to open file"; 
}

//Generate the pool of letters to be played with in accordance with the English Scrabble letter frequency
vector<letterTile> Scrabble::generateLetterPool(int numberOfPlayers) {
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
	vector<letterTile> letterPool;
	int cycler = 0;
	int numberOfTiles = numberOfPlayers * 25;
	//Calculate a full set of letters for each player, then shuffle, then prune 3/4 of them
	//That way the letters are evenly distributed
	int numberOfCycles = numberOfPlayers * 100;


	//Go around the letter list adding letters until enough tiles have been pushed
	for (int i = 0; i < numberOfCycles; i++) {
		letterTile tmp;								//Create a generic letter tile 
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

	return letterPool;
}

//Draw several letters from the pool and add them to a player's hand
vector<letterTile> Scrabble::drawLettersAndAddToHand(int numberOfLetters, vector<letterTile>* hand, vector<letterTile>* pool) {
	vector<letterTile> newHand = *hand;
	//Push fresh pool values in to the hand
	for (int i = 0; i < numberOfLetters; i++) {
		newHand.push_back(drawLetterChar(pool));
	}
	//Return the player's new hand
	return newHand;
}

//Gets a letter from the pool and removes it from the pool 
letterTile Scrabble::drawLetterChar(vector<letterTile> * pool) {
	letterTile returnVal = pool->back();		//Get the last element
	pool->pop_back();					//Pop the last element
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
		"Error, invalid character";
		return -1;
	}
}

//TODO: Checks if a word can be formed using the existing letters on the board and the player�s tiles
bool Scrabble::canFormWord(proposedWord w, vector<letterTile> playerHand) {
	return true;
}

//TODO: Returns the value of placing a word at a certain position on the game board
int Scrabble::getWordValue(proposedWord w) {
	return 1;
}

//TODO: Receives a pointer to the board matrix and a number of players
//Generates the board based on the player number and assigns values to the board variable
void Scrabble::generateBoard(int players) {
	int boardSize = 15;						//Standard board size for 4 players
	boardSize = boardSize / 4 * players;	//Adjust it for our player number
	//If the board size comes out even, increase it by one
	if (boardSize % 2 == 0)
		boardSize++;

	//Allocate memory to the board
	board = (char**)malloc(boardSize * sizeof(char*));			//Cast necessary because of the C++ compiler
	for (int i = 0; i < boardSize; i++) {
		board[i] = (char*)malloc(boardSize * sizeof(char));
	}

	//Assign tile roles
	//0 = Regular tile
	//1 = Double letter score
	//2 = Triple letter score
	//3 = Double word score
	//4 = Triple word score
	//Any other char = Placed letter(ex.A, B, C, etc.)


	//Set all positions of the board to be regular tiles
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			if (i == j) {
				//Double word
				board[i][j] = '3';
			}
			else {
				//Regular tile
				board[i][j] = '0';
			}
		}

		//Central multipliers
		board[boardSize / 2 + 1][boardSize / 2 + 1] = '1';
		board[boardSize / 2 + 1][boardSize / 2 - 1] = '1';
		board[boardSize / 2 - 1][boardSize / 2 + 1] = '1';
		board[boardSize / 2 - 1][boardSize / 2 - 1] = '1';

		board[boardSize / 2 + 2][boardSize / 2 + 2] = '2';
		board[boardSize / 2 + 2][boardSize / 2 - 2] = '2';
		board[boardSize / 2 - 2][boardSize / 2 + 2] = '2';
		board[boardSize / 2 - 2][boardSize / 2 - 2] = '2';

		//Triple word corners
		board[0][0] = '4';
		board[0][boardSize - 1] = '4';
		board[boardSize - 1][0] = '4';
		board[boardSize - 1][boardSize - 1] = '4';

		//Triple word centres
		board[0][boardSize / 2] = '4';
		board[boardSize - 1][boardSize / 2] = '4';
		board[boardSize / 2][boardSize - 1] = '4';
		board[boardSize / 2][0] = '4';
	}

	printBoard(players);

}

//Print board
void Scrabble::printBoard(int players) {
	int boardSize = 15;						//Standard board size for 4 players
	boardSize = boardSize / 4 * players;	//Adjust it for our player number
	//If the board size comes out even, increase it by one
	if (boardSize % 2 == 0)
		boardSize++;

	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
}

//Free board memory
void Scrabble::freeBoard(int players) {
	int boardSize = 15;						//Standard board size for 4 players
	boardSize = boardSize / 4 * players;	//Adjust it for our player number
	//If the board size comes out even, increase it by one
	if (boardSize % 2 == 0)
		boardSize++;

	for (int i = 0; i < boardSize; i++) {
		free(board[i]);
	}
	free(board);
}