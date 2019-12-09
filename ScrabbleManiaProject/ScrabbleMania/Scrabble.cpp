/*
Luis Barajas
Eric Parton
*/

#include "Scrabble.h"
#include "trim.h"

#define HAND_SIZE 7

#define STRING_SIZE 100
#define BUFFER_SIZE 2048

// Constructor
Scrabble::Scrabble(){
	hasActiveGame = false;
	isFirstGameTurn = true;
	board = NULL;
	gameSettings.playerNumber = 0;
	turn = 0;
}

// New player wants to connect to game
int Scrabble::addPlayerToGame(string name){
	int id = -1;
	if (!hasActiveGame){
		// The id will be its index at players' vector
		id = players.size();
		Player newPlayer = Player(id, name, ACTIVE);
		players.push_back(newPlayer);
	}else{
		// Do not add it but let it spectate
		id = players.size();
		Player newPlayer = Player(id, name, SPECTATOR);
		players.push_back(newPlayer);
	}
	return id;
}

// Get added words as string
string Scrabble::getAddedWords(){
	char words[BUFFER_SIZE] = "";

	// loop through addedWords vector
	for(int i = 0; i < addedWords.size(); i++){
		proposedWord_t addedWord = addedWords[i];
		char wordString[STRING_SIZE];
		// Append added word to string with format: word start.y start.x direction
		sprintf(wordString, "%s %d %d %c:", (char *)addedWord.word.c_str(), addedWord.start.y, addedWord.start.x, addedWord.direction);
		strcat(words, wordString);
	}

	// Remove last : added
	words[strlen(words)-1] = '\0';
	// Convert char * to string
	string wordString(words);

	return wordString;
}

// Set dictionary wanted by user
void Scrabble::setDictionary(string dictionaryFileName){
	gameSettings.dictionaryFileName = dictionaryFileName;
}

// Start the game
void Scrabble::startGame(){
	if(!hasActiveGame) {

		fillDictionary();
		generateBoard();
		generateLetterPool();
		printBoard();

		// Give 7 letters to each player
		for(int i = 0; (unsigned)i < gameSettings.playerNumber; i++){
			drawLettersAndAddToHand(HAND_SIZE, i);
		}

		hasActiveGame = true;
	}
}

// End the game
void Scrabble::endGame(){
	if(hasActiveGame) {
		hasActiveGame = false;
		updateScoreboard();

		freeBoard();

		// Rest turn
		turn = 0;

		// Reset gameSettings
		gameSettings.dictionaryFileName = "";
		gameSettings.playerNumber = 0;

	}
}

// Get the name of the player
string Scrabble::getPlayerName(int playerId){
	return players[playerId].getName();
}

// Check if player is still in playing
bool Scrabble::isPlayerStillInGame(int playerId){
	return players[playerId].stillInGame;
}

// Player is leaving game, return his position, score and who is winning
string Scrabble::playerIsLeaving(int playerId){
	// Set player's stillInGame to false
	players[playerId].stillInGame = false;

	char buffer[STRING_SIZE];

	// Create string with format: "(player's scoreboard position) (player's score) (first place player's name)"
	sprintf(buffer, "%d %d %s", playerPosition(playerId), players[playerId].getScore(), (char *)players[scoreBoard[0]].getName().c_str());
	// Convert char * to string
	string playerPositionScore(buffer);

	return playerPositionScore;

}

// Player position depending on score
int Scrabble::playerPosition(int playerId){
 
	// Find given element in vector
	vector<int>::iterator it = std::find(scoreBoard.begin(), scoreBoard.end(), playerId);
	// Calculate index from beginning of vector
	return distance(scoreBoard.begin(), it);
}

// Compares two players according to score
bool comparePlayerScore(Player i1, Player i2) { 
	return (i1.getScore() > i2.getScore()); 
}

// Get calculate scoreBoard
void Scrabble::updateScoreboard(){
	scoreBoard.clear();
	vector<Player> players_copy(players);
	
	// Sort copy of vector using score
	sort(players_copy.begin(), players_copy.end(), comparePlayerScore);

	// Create scoreboard, the value is the id of the player at that position
	for(int i = 0; i < players_copy.size(); i++){
		scoreBoard.push_back(players_copy[i].getId());
	}
}


// Get the number of active players
int Scrabble::countActivePlayers(){
	int count = 0;
	for(int i = 0; (unsigned)i < players.size(); i++){
		// Increment count if player is still in game
		if(players[i].stillInGame){
			count++;
		}
	}
	return count;
}

// Get the number of players
int Scrabble::getPlayerNumber(){
	return players.size();
}

//Returns a set of letters in scrabble tiles
string Scrabble::getHand(int playerId){
	string handString = "";

	vector<letterTile_t> *hand = players[playerId].getHand();
	for (int i = 0; (unsigned)i < hand->size(); i++) {
		// Append letter of tile to string
		handString += hand->at(i).letter;
	}

	return handString;
}

//Prints a set of scrabble tiles
void Scrabble::printHand(int playerId) {
	players[playerId].printHand();
}

// Fill dictionary with the settings from the user
void Scrabble::fillDictionary(){
	string line;
	ifstream file (gameSettings.dictionaryFileName);

	if (file.is_open()) {
	while ( getline (file,line) ) {
		// Save it in a the dictionary data structure
		trim(line);
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
	int numberOfTiles = players.size() > 4 ? players.size() * 25 : 100; // Minimum 100 tiles
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
		if((unsigned)cycler >= sizeof(letterList)/sizeof(*letterList))
			cycler = 0;
	}

	//Shuffle the pool
	random_shuffle(letterPool.begin(), letterPool.end());

	//Prune the extra tiles
	letterPool.resize(numberOfTiles);
}

// Exchange specific letters from hand for random letters of letter pool
void Scrabble::exchangeLetters(int playerId, vector<int> indexes){
	// Get the hand from playerId
	vector<letterTile_t> *hand = players[playerId].getHand();
	int index = 0;
	letterTile_t letterTile;

	int numberOfLetters = indexes.size();

	//Make sure there are enough letters to draw
	if((unsigned)numberOfLetters > letterPool.size()) {
		numberOfLetters = letterPool.size();
	}

	// Loop through the received indexes of tiles
	for(int i = 0; (unsigned)i < numberOfLetters; i++) {
		index = indexes[i];
		// Get the tile
		letterTile = hand->at(index);
		// Return the tile to the letterPool
		letterPool.insert(letterPool.begin(), letterTile);
		// Remove it from the hand
		hand->erase(hand->begin() + index);
		// Draw a new letter to the hand at the same position
		hand->insert(hand->begin() +  index, drawLetterChar());
	}

	//Shuffle the pool
	random_shuffle(letterPool.begin(), letterPool.end());
}

//Draw several letters from the pool and add them to a player's hand
void Scrabble::drawLettersAndAddToHand(int numberOfLetters, int playerId) {
	//Make sure there are enough letters to draw
	if((unsigned)numberOfLetters > letterPool.size()) {
		numberOfLetters = letterPool.size();
	}
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

// Adds word to board if it's a valid one
int Scrabble::addWordToGame(proposedWord_t proposedWord, int playerId) {
	if(!isValidWord(proposedWord, playerId)){
		return -1;
	}

	//Write word onto board
	placeLettersOnBoard(proposedWord);

	// Save word in vector
	addedWords.push_back(proposedWord);

	//Add points from formed word to player
	int wordValue = getWordValue(proposedWord);
	cout << proposedWord.word << " scores " << wordValue << endl;
	players[playerId].setScore(players[playerId].getScore() + wordValue);


	// Check if this was the first game turn
	if(isFirstGameTurn){
		isFirstGameTurn = false;
	}

	return wordValue;
}

//Write word onto board
void Scrabble::placeLettersOnBoard(proposedWord_t proposedWord) {
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

/*
 * Checks if a word can be formed:
 * 1. Exists in the dictionary (isInDictionary function)
 * 2. Fits in the board considering the existing letters on it (canFormWord function)
 * 3. The player's tiles (canFormWord function)
 *
 */
bool Scrabble::isValidWord(proposedWord_t proposedWord, int playerId) {
	cout << proposedWord.word << " is in dictionary: " << isInDictionary(proposedWord.word) << endl;
	return isInDictionary(proposedWord.word) && canFormWord(proposedWord, playerId);
}

// Checks if a word can be formed using the existing letters on the board and the player's tiles
bool Scrabble::canFormWord(proposedWord_t proposedWord, int playerId) {
	vector<char> neededLetters;
	// Check if words fits in board and if there are letters already in position, that they are equal to the index in word
	if (!wordFitsInBoard(proposedWord, &neededLetters)){
		cout << "Error, invalid word placement" << endl;
		return false;
	}

	// Check if is a valid word, using at least a letter of other word if it's not the first game turn
	if(!isFirstGameTurn && (proposedWord.word.length() <= neededLetters.size())){
		cout << "Error: invalid word, it should use at least one letter from another word!" << endl;
		return false;
	}

	// Check if player has the missing letters
	if(!players[playerId].hasNeededLetters(neededLetters)){
		cout << "Error: " << players[playerId].getName() << " doesn't have the necessary letters!" << endl;
		return false;
	}

	//Remove the tiles from the player's hand
	removeTilesFromHand(playerId, neededLetters);

	//Add new tiles to the player's hand
	drawLettersAndAddToHand(neededLetters.size(), playerId);

	return true;
}

//Remove the tiles from the player's hand
void Scrabble::removeTilesFromHand(int playerId, vector<char> neededLetters) {
		for(int i = 0; (unsigned)i < neededLetters.size(); i++) {
			players[playerId].removeTileFromHand(neededLetters[i]);
		}
}

// Get letter pool size
int Scrabble::getLetterPoolSize(){
	return letterPool.size();
}

// Check if words fits in board
// Check if there are letters already in position, that they are equal to the index in word
// Fill the vector of neededLetters
bool Scrabble::wordFitsInBoard(proposedWord_t proposedWord, vector<char> *neededLetters){
	// Check if words fits in board
	if(proposedWord.start.x >= board->size || proposedWord.start.y >= board->size){
		printf("Error: start coordinate of proposed word must be within the board limits\n");
		return false;
	}

	// Check if there are letters already in position, that they are equal to the index in word
	int coordinateLastLetter = 0;
	int i = 0;
	switch (proposedWord.direction){
		/*case 'u':
			// if up, last letter coordinate will be in coordinate y
			coordinateLastLetter = proposedWord.start.y;
			while(coordinateLastLetter > -1 && (unsigned)i < proposedWord.word.size()){
				if(board->data[proposedWord.start.x][coordinateLastLetter] < '5'){
					// It means it is empty with value of 0,1,2,3,4
					// Add letter to neededLetters vector
					neededLetters->push_back(proposedWord.word[i]);
				// Check that the letter that is in tile is the same of word index
				}else if(board->data[proposedWord.start.x][coordinateLastLetter] != proposedWord.word[i]){
					return false;
				}

				coordinateLastLetter--;
				i++;
			}
		break;*/
		case 'd':
			// if down, last letter coordinate will be in coordinate y
			coordinateLastLetter = proposedWord.start.y;
			while(coordinateLastLetter < board->size && (unsigned)i < proposedWord.word.size()){
				if(board->data[proposedWord.start.x][coordinateLastLetter] < '5'){
					// It means it is empty with value of 0,1,2,3,4
					// Add letter to neededLetters vector
					neededLetters->push_back(proposedWord.word[i]);
				// Check that the letter that is in tile is the same of word index
				}else if(board->data[proposedWord.start.x][coordinateLastLetter] != proposedWord.word[i]){
					return false;
				}

				coordinateLastLetter++;
				i++;
			}
		break;
		/*case 'l':
			// if left, last letter coordinate will be in coordinate x
			coordinateLastLetter = proposedWord.start.x;
			while(coordinateLastLetter > -1 && (unsigned)i < proposedWord.word.size()){
				if(board->data[coordinateLastLetter][proposedWord.start.y] < '5'){
					// It means it is empty with value of 0,1,2,3,4
					// Add letter to neededLetters vector
					neededLetters->push_back(proposedWord.word[i]);
				// Check that the letter that is in tile is the same of word index
				}else if(board->data[coordinateLastLetter][proposedWord.start.y] != proposedWord.word[i]){
					return false;
				}

				coordinateLastLetter--;
				i++;
			}
		break;*/
		case 'r':
			// if right, last letter coordinate will be in coordinate x
			coordinateLastLetter = proposedWord.start.x;
			while(coordinateLastLetter < board->size && (unsigned)i < proposedWord.word.size()){
				if(board->data[coordinateLastLetter][proposedWord.start.y] < '5'){
					// It means it is empty with value of 0,1,2,3,4
					// Add letter to neededLetters vector
					neededLetters->push_back(proposedWord.word[i]);
				// Check that the letter that is in tile is the same of word index
				}else if(board->data[coordinateLastLetter][proposedWord.start.y] != proposedWord.word[i]){
					return false;
				}

				coordinateLastLetter++;
				i++;
			}
		break;
		default:
		break;
	}

	// It means the word does not fit in the board's bounds
	if((unsigned)i < proposedWord.word.size()){
		printf("Error: proposed word must be within the board limits\n");
		return false;
	}
	return true;
}

//Returns the value of placing a word at a certain position on the game board
int Scrabble::getWordValue(proposedWord_t proposedWord) {
	//Get the vector of values on the existing board where the word will be placed
	vector<char> boardWordPath = getBoardWordPath(proposedWord);
	//Calculate the value of the word base on where it's being placed
	return getWordValueWithBonuses(proposedWord.word, boardWordPath);
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

	//Centre of the board is neutral
	board->data[board->size / 2][board->size / 2] = '0';
}

//Print board
void Scrabble::printBoard() {

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
int Scrabble::getBoardSize(){
	return board->size;
}

// Print horizontal board separator
void Scrabble::printBoardSeparator(){
	printf("    ");
	for(int i = 0; i < board->size; i++){
		printf(" ---");
	}
	printf("\n");
}

//Get the char value from the board at a certain position
char Scrabble::getBoardPosValue(int x, int y) {
	return board->data[x][y];
}

//Set the board value at a position
void Scrabble::setBoardPosValue(int x, int y, char value) {
	board->data[x][y] = value;
}

//Free board memory
void Scrabble::freeBoard() {
	for(int i = 0; i < board->size; i++){
    free(board->data[i]);
  }
  free(board->data);
  free(board);
}

//Get the vector of values on the existing board where the word will be placed
vector<char> Scrabble::getBoardWordPath(proposedWord_t proposedWord) {
	//Simplify variable names
	int x = proposedWord.start.x;
	int y = proposedWord.start.y;
	string word = proposedWord.word;
	char direction = proposedWord.direction;

	vector<char> boardWordPath;
	for (int i = 0; (unsigned)i < word.length(); i++) {
		//Push the value and move to the next board position
		boardWordPath.push_back(getBoardPosValue(x, y));
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
	return boardWordPath;
}

//Returns the value of a word, taking in to account bonuses
int Scrabble::getWordValueWithBonuses(string word, vector<char> boardWordPath) {
	int value = 0;
	int letterValue = 0;
	int wordMultiplier = 1;
	for (int i = 0; (unsigned)i < word.length(); i++) {
		//Get the value of the letter
		letterValue = getLetterValue(word[i]);
		//Check if it can be multiplied by anything
		switch (boardWordPath[i]) {
		case '1':
			//Double letter score
			letterValue *= 2;
			break;
		case '2':
			//Triple letter score
			letterValue *= 3;
			break;
		case '3':
			//Double word score
			wordMultiplier *= 2;
			break;
		case '4':
			//Triple word score
			wordMultiplier *= 3;
			break;
		default:
			//Nothing interesting (0 tile or already placed letter)
			break;
		}
		//Add the letter value after each iteration
		value += letterValue;
	}
	//Multiply by any word multipliers at the end
	value *= wordMultiplier;
	return value;
}

void Scrabble::printScores() {
	for (int i = 0; (unsigned)i < players.size(); i++) {
		players[i].printScore();
	}
}

void Scrabble::setSettingsPlayerNumber(int pn) {
	gameSettings.playerNumber = pn;
}

int Scrabble::getSettingsPlayerNumber() {
	return gameSettings.playerNumber;
}

// Get turn
int Scrabble::getTurn(){
	return turn;
}

// Next turn
void Scrabble::nextTurn(){
	turn++;
}
