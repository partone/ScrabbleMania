/*
Luis Barajas
Eric Parton
*/

#include "Scrabble.h"

//Prints a set of scrabble tiles
void printHand(vector<letterTile> hand) {
	 for (int i = 0; i < hand.size(); i++) {
		 cout << hand[i].letter << " " << hand[i].value;
		 if (i != hand.size() - 1) {
			 cout << " // ";
		 }
	 }
	 cout << endl;
}

//Generate the pool of letters to be played with in accordance with the English Scrabble letter frequency
vector<letterTile> generateLetterPool(int numberOfPlayers) {
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
vector<letterTile> drawLettersAndAddToHand(int numberOfLetters, vector<letterTile>* hand, vector<letterTile>* pool) {
	vector<letterTile> newHand = *hand;
	//Push fresh pool values in to the hand
	for (int i = 0; i < numberOfLetters; i++) {
		newHand.push_back(drawLetterChar(pool));
	}
	//Return the player's new hand
	return newHand;
}

//Gets a letter from the pool and removes it from the pool 
letterTile drawLetterChar(vector<letterTile> * pool) {
	letterTile returnVal = pool->back();		//Get the last element
	pool->pop_back();					//Pop the last element
	return returnVal;					//Return the letter
}

//TODO: Checks if a word exists in a given dictionary, returns true if it does.  False otherwise.
bool isInDictionary(string s, string dictionary) {
	return true;
}

//Returns the point value of a letter in accordance with English Scrabble letter points
int getLetterValue(char c) {
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

//TODO: Checks if a word can be formed using the existing letters on the board and the player’s tiles
bool canFormWord(proposedWord w, char* board, vector<letterTile> playerHand) {
	return true;
}

//TODO: Returns the value of placing a word at a certain position on the game board
int getWordValue(proposedWord w, char* board) {
	return 1;
}

//TODO: Receives a pointer to the board matrix and a number of players
//Generates the board based on the player number and assigns values to the board variable
void generateBoard(char* board, int players) {

}