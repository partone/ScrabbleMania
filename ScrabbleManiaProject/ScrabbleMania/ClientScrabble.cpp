/*
Luis Barajas
Eric Parton
*/

#include "ClientScrabble.h"

// Constructor
ClientScrabble::ClientScrabble(){
  playersNumber = 0;
}

void ClientScrabble::setPlayersNumber(int _playersNumber){
  playersNumber = _playersNumber;
}
int ClientScrabble::getPlayersNumber(){
  return playersNumber;
}

//Returns the point value of a letter in accordance with English Scrabble letter points
int ClientScrabble::getLetterValue(char c){
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

// Adds word to board
void ClientScrabble::addWordToGame(proposedWord_t proposedWord){
  //Simplify variable names
	int x = proposedWord.start.x;
	int y = proposedWord.start.y;
	string word = proposedWord.word;
	char direction = proposedWord.direction;

	vector<char> boardWordPath;
	for (int i = 0; (unsigned)i < word.length(); i++) {
		//Place the letter at the board position
		setBoardPosValue(x, y, word[i]);
		//Move to the next index
		switch (direction) {
		case 'u':
			y--;
			break;
		case 'd':
			y++;
			break;
		case 'l':
			x--;
			break;
		case 'r':
			x++;
			break;
		default:
			cout << "Error: invalid direction\n";
			break;
		}
	}
	return;
}

//Set the board value at a position
void ClientScrabble::setBoardPosValue(int x, int y, char value) {
	board->data[x][y] = value;
}

//Print board
void ClientScrabble::printBoard(){

	/*
	Sample
	  9   10  11
	 --- --- ---
	|   | 3 | t |
	 --- --- ---
	|   |   | h |
	 --- --- ---
	|   |   | e |
	 --- --- ---

	*/

	// print headings
	printf("\n");
	printf("    ");
	for(int i = 0; i < board->size; i++){
		printf("  %d", i+1);
		if(i+1 < 10) {
			printf(" ");
		}
	}
	printf("\n");

	printBoardSeparator();

	// Print board
	for(int i = 0; i < board->size; i++){
		printf("%2d  ", i+1);

		for(int j = 0; j < board->size; j++){
			printf("| %c ", board->data[j][i] != '0' ? board->data[j][i] : ' ');
		}

		printf("|\n");
		printBoardSeparator();
	}

	printf("\n");
}

// Get board size
int ClientScrabble::getBoardSize(){
	return board->size;
}

// Print horizontal board separator
void ClientScrabble::printBoardSeparator(){
  printf("    ");
	for(int i = 0; i < board->size; i++){
		printf(" ---");
	}
	printf("\n");
}

//Generates the board based on the player number and assigns values to the board variable
void ClientScrabble::generateBoard(){
  int boardSize = 15;						//Standard board size for 4 players

	board = (board_t *)malloc(sizeof(board_t));
	board->data = NULL;
	board->size = playersNumber > 4 ? boardSize / 4 * playersNumber : boardSize;	//Adjust it for our player number if it is greater than 4
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

	//Centre of the board is neutral
	board->data[board->size / 2][board->size / 2] = '0';
}



//Free board memory
void ClientScrabble::freeBoard(){
  for(int i = 0; i < board->size; i++){
    free(board->data[i]);
  }
  free(board->data);
  free(board);
}
