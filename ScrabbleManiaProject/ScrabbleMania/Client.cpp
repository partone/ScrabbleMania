/*
Luis Barajas
Eric Parton
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>
// Custom libraries
#include "sockets.h"
#include "fatal_error.h"

#include "Player.h"
#include "ClientScrabble.h"

using namespace std;

#define BUFFER_SIZE 1024
#define BUFFER_LONG_SIZE 2048

// Function declarations
void usage(char * program);
void playGame(int connection_fd, bool *interrupted);
void initializeGame(int connection_fd, char *buffer, ClientScrabble *scrabble, Player *player, int *playersNumber, int *playerID);
void playerTurn(int connection_fd, char *buffer, ClientScrabble *scrabble, Player *player);
void gameEnded(int connection_fd, char *buffer);
void spectating(int connection_fd, char *buffer, ClientScrabble *scrabble);

void addWord(int connection_fd, ClientScrabble *scrabble);
void exchangeTiles(int connection_fd, ClientScrabble *scrabble, Player *player);

int main(int argc, char * argv[]) {
	int connection_fd;
	bool interrupted = false;

	printf("\n=== Welcome to ClientScrabble ===\n");

	// Check the correct arguments
	if (argc != 3)
	{
			usage(argv[0]);
	}

	// Connect to the server
	connection_fd = connectSocket(argv[1], argv[2]);
	playGame(connection_fd, &interrupted);

	// Close the socket
	close(connection_fd);

	return 0;
}

// Game playing function
void playGame(int connection_fd, bool *interrupted) {
	char buffer[BUFFER_SIZE];
	// Simple scrabble board without any validations only to add words that have already been validated in server
	ClientScrabble scrabble = ClientScrabble();

	cout << "Connection successful, creating thread\n";
	//Wait for thread confirmation
	recvString(connection_fd, buffer, BUFFER_SIZE);
	cout << "Welcome to Scrabble MANIA!\n";
	cout << "Whenever you would like to quit press CTRL+C\n";
	//Ask user for name
	string name;
	cout << "What is your name?\n";
	cin >> name;

	// Send name
	sprintf(buffer, "%s", (char *)name.c_str());
	sendString(connection_fd, buffer, strlen(buffer)+1);

	int playersNumber = 0;
	int playerID = 0;

	// Send number of players for this game, if client is the first to connect
	recvString(connection_fd, buffer, BUFFER_SIZE);
	if(!strcmp(buffer, "SEND_PLAYER_NUMBER")){
		//It was the first player, ask for number of players
		cout << "You are the first player in the Lobby. How many players will this game consist of?" << endl;
		cin >> playersNumber;

		// Send players number
		sprintf(buffer, "%d", playersNumber);
		sendString(connection_fd, buffer, strlen(buffer)+1);
	}else{
		// Receive players number and playerID
		sscanf(buffer, "%d %d", &playersNumber, &playerID);

		// Send OK
		sprintf(buffer, "OK");
		sendString(connection_fd, buffer, strlen(buffer)+1);
	}

	// Create player with id and name
	Player player = Player(playerID, name);

	initializeGame(connection_fd, buffer, &scrabble, &player, &playersNumber, &playerID);

	// Game loop
	while(!*interrupted){
		// Receive turn communication
		recvString(connection_fd, buffer, BUFFER_SIZE);

		if(!strcmp(buffer, "YOUR_TURN")){

			playerTurn(connection_fd, buffer, &scrabble, &player);

		}else if(!strcmp(buffer, "GAME_ENDED")){
			
			gameEnded(connection_fd, buffer);

		}else{
			
			spectating(connection_fd, buffer, &scrabble);

		}

	}

	scrabble.freeBoard();
}

// Initialize game
void initializeGame(int connection_fd, char *buffer, ClientScrabble *scrabble, Player *player, int *playersNumber, int *playerID){

	cout << "This game will consist of " << *playersNumber << " players" << endl;

	if(*playerID < *playersNumber - 1){
		//Wait for confirmation that the other players are connected
		cout << "Waiting for other players...\n";
	}

	scrabble->setPlayersNumber(*playersNumber);

	scrabble->generateBoard();

	if(*playerID >= *playersNumber){
		// Spectator
		cout << "YOU ARE ONLY A SPECTATOR!!" << endl;

		// Receives words already added 
		recvString(connection_fd, buffer, BUFFER_LONG_SIZE);

		char *str = strtok(buffer, ":");
		while(str != NULL){
			// Word
			char *word = new char[scrabble->getBoardSize() + 1];
			int x;
			int y;
			char direction;
			
			sscanf(str, "%s %d %d %c", word, &x, &y, &direction);

			string wordString(word);

			proposedWord_t proposedWord = proposedWord_t(wordString, x, y, direction);
			scrabble->addWordToGame(proposedWord);

			str = strtok(NULL, ":");
		}

	}else{
		string handString;
		// Receives hand
		recvString(connection_fd, buffer, BUFFER_SIZE);

		// Separate by char
		vector<letterTile_t> hand;

		for(int i = 0; (unsigned)i < strlen(buffer); i++){
			hand.push_back(letterTile_t(buffer[i], scrabble->getLetterValue(buffer[i])));
		}

		player->setHand(hand);
	}

	cout << "Game Started!" << endl;

	sprintf(buffer, "OK");
	sendString(connection_fd, buffer, strlen(buffer)+1);

	scrabble->printBoard();
}

// Turn of this client player
void playerTurn(int connection_fd, char *buffer, ClientScrabble *scrabble, Player *player){
	
	// Send OK
	sprintf(buffer, "OK");
	sendString(connection_fd, buffer, strlen(buffer)+1);
	
	// Receive hand
	recvString(connection_fd, buffer, BUFFER_SIZE);
	
	// Separate by char
	vector<letterTile_t> hand;

	for(int i = 0; (unsigned)i < strlen(buffer); i++){
		hand.push_back(letterTile_t(buffer[i], scrabble->getLetterValue(buffer[i])));
	}
	
	player->setHand(hand);
	
	// Turn of current client
	cout << "It's your turn! Your hand is: " << endl;
	player->printHand();

	int ans = 0;
	while(ans != 1 && ans != 2){
		cout << "What would you like to do?" << endl;
		cout << "\t1. Add a word to board" << endl;
		cout << "\t2. Change tiles" << endl;

		cin >> ans;

		switch(ans) {
			case 1:
				addWord(connection_fd, scrabble);
			break;
			case 2:
				exchangeTiles(connection_fd, scrabble, player);
			break;
			default:
				cout << "Invalid option please enter a valid one!" << endl;
			break;
		}
	}

	// Send OK
	sprintf(buffer, "OK");
	sendString(connection_fd, buffer, strlen(buffer)+1);
}

// Game ended when someone won
void gameEnded(int connection_fd, char *buffer){
	// Game ended
	sprintf(buffer, "OK");
	sendString(connection_fd, buffer, strlen(buffer)+1);

	// Receives positon and score
	int position = 0;
	int score = 0;
	char winningPlayer[BUFFER_SIZE];
	
	recvString(connection_fd, buffer, BUFFER_SIZE);
	sscanf(buffer, "%d %d %s", &position, &score, winningPlayer);

	cout << "The game has ended!" << endl;

	if(position == 0){
		cout << "Congratulations!!!" << endl;
		cout << "You were the first place with a score of " << score << " points!" << endl;
	}else{
		cout << winningPlayer << " won!" << endl;
		cout << "You ended at position " << position+1 << " with a score of " << score << " points!" << endl;
	}

	cout << "Thank you for playing with us!" << endl;
}

// Spectating someone else's turn
void spectating(int connection_fd, char *buffer, ClientScrabble *scrabble) {
	// Received player's turn name
	string playersNameTurn(buffer);

	sprintf(buffer, "OK");
	sendString(connection_fd, buffer, strlen(buffer)+1);

	cout << playersNameTurn << "'s turn" << endl;
	cout << "Waiting for their move" << endl;

	recvString(connection_fd, buffer, BUFFER_SIZE);

	// Check what was the move
	if(!strcmp(buffer, "CHANGED")){
		cout << playersNameTurn << " exchanged tiles." << endl;
		// Player only chnaged tiles
		sprintf(buffer, "OK");
		sendString(connection_fd, buffer, strlen(buffer)+1);
	}else{
		// Player added word
		char *word = new char[scrabble->getBoardSize() + 1];
		int x;
		int y;
		char direction;

		sscanf(buffer, "%s %d %d %c", word, &x, &y, &direction);

		string wordString(word);

		cout << playersNameTurn << " added:" << endl;
		cout << "\tWord: " << wordString << endl;
		cout << "\tCoordinates: " << y+1 << ", " << x+1 << endl;
		cout << "\tDirection: " << direction << endl;

		proposedWord_t proposedWord = proposedWord_t(wordString, x, y, direction);
		scrabble->addWordToGame(proposedWord);

		scrabble->printBoard();

		sprintf(buffer, "OK");
		sendString(connection_fd, buffer, strlen(buffer)+1);
	}
}

// Add word to game
void addWord(int connection_fd, ClientScrabble *scrabble) {
	// Set up function variables
	char buffer[BUFFER_SIZE];
	int wordValue = 0;
	string word;
	int x;
	int y;
	char direction;

	// Continue asking for word until there is a valid one
	while(1){
		cout << "Let's add a new word!" << endl;
		cout << "Which word is in your mind? (Remember to write it completely although there are already some letters on the board)" << endl;
		cin >> word;

		cout << "X coordinate?" << endl;
		cin >> x;

		cout << "Y coordinate?" << endl;
		cin >> y;

		cout << "Which direction will it be facing? (d: down, r: right)" << endl;
		cin >> direction;

		// Send word with format: "word start.y start.x direction"
		sprintf(buffer, "%s %d %d %c", (char *)word.c_str(), y - 1, x - 1, direction);
		sendString(connection_fd, buffer, strlen(buffer)+1);

		// Recieve outcome of adding word
		recvString(connection_fd, buffer, BUFFER_SIZE);
		if(strcmp(buffer, "INVALID")){
			// Was succesful, received word value
			sscanf(buffer, "%d", &wordValue);
			break;
		}else{
			// Was invalid
			cout << "Invalid word please try again!" << endl;
		}
	}

	cout << "Successfully added word to board! The word was good for " << wordValue << " points." << endl;
	proposedWord_t proposedWord = proposedWord_t(word, y - 1, x - 1, direction);

	// Add word to local word
	scrabble->addWordToGame(proposedWord);

	scrabble->printBoard();

}

void exchangeTiles(int connection_fd, ClientScrabble *scrabble, Player *player) {
	// Set up function variables
	char buffer[BUFFER_SIZE];

	// send change
	sprintf(buffer, "CHANGE");
	sendString(connection_fd, buffer, strlen(buffer)+1);

	// Receive OK
	recvString(connection_fd, buffer, BUFFER_SIZE);

	string tiles;
	cout << "Enter the indexes of the tiles separated by commas (without spaces)" << endl;
	cin >> tiles;

	// Send the tiles' indexes 
	sprintf(buffer, "%s", (char *)tiles.c_str());
	sendString(connection_fd, buffer, strlen(buffer)+1);

	string handString;

	// Receives modified hand
	recvString(connection_fd, buffer, BUFFER_SIZE);

	vector<letterTile_t> hand;

	for(int i = 0; (unsigned)i < strlen(buffer); i++){
		hand.push_back(letterTile_t(buffer[i], scrabble->getLetterValue(buffer[i])));
	}

	// Set hand for the player
	player->setHand(hand);

	player->printHand();
}

void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}

// TODO: read CTRL+C, when client uses it, he quits, display to him his current score and position. Notify server.
