/*
Luis Barajas
Eric Parton
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// Sockets libraries
#include <netdb.h>
#include <sys/poll.h>
// Posix threads library
#include <pthread.h>

#include "sockets.h"
#include "Scrabble.h"
#include "Player.h"
#include <sstream>


#define HAND_SIZE 7
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024
#define BUFFER_LONG_SIZE 2048

// Function declarations
void waitForConnections(int server_fd, Scrabble *scrabble);
void * clientHandler(void * arg);
void usage(char * program);

//The struct to be sent to the thread
typedef struct clientThreadDataStruct {
	Scrabble * scrabble;
	// Word that was just successfully added
	bool * playerMadeMove;
	bool * gameHasNewWord;
	proposedWord_t * addedWord;

	// Concurrency
	//Declaration of thread condition variable
	pthread_cond_t * playerTurnOverCond;
	//Mutex for the player turn
	pthread_mutex_t * lock;

	int connection_fd;
} clientThreadData;

int main(int argc, char * argv[]) {
	// Check the correct arguments
	if (argc != 2)
	{
			usage(argv[0]);
	}

	srand(time(0));			//Initialise seed for random stuff
	Scrabble scrabble = Scrabble();

	// TODO: ask this to the first player
	scrabble.setDictionary("dictionaries/english.txt");

	//Set up the sever
	int server_fd;
	server_fd = initServer(argv[1], MAX_CLIENTS);

	// Listen for connections from the clients
	waitForConnections(server_fd, &scrabble);

	return 1;
}

void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}


//Server is running, waiting for player to start game
void waitForConnections(int server_fd, Scrabble *scrabble)
{

	// Set up variables for thread data
	bool playerMadeMove = false;
	bool gameHasNewWord = false;
	proposedWord_t addedWord;
	pthread_cond_t playerTurnOverCond = PTHREAD_COND_INITIALIZER;
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	
	struct sockaddr_in client_address;
	socklen_t client_address_size;
	char client_presentation[INET_ADDRSTRLEN];
	int client_fd;
	pthread_t new_tid;
	clientThreadData * ctd = NULL;		//For the data sent to the thread

	//Allow each player to connect to the game
	while(1){
		// Get the size of the structure to store client information
		client_address_size = sizeof client_address;
		while(1) {
			// Wait for a client connection
			client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_size);
			// Get the data from the client
			inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
			printf("Received incoming connection from %s on port %d\n", client_presentation, client_address.sin_port);
			break;
		}
		//Prepare the client struct
		ctd = (clientThreadData *)malloc(sizeof(clientThreadData));
		ctd->scrabble = scrabble;
		ctd->playerMadeMove = &playerMadeMove;
		ctd->gameHasNewWord = &gameHasNewWord;
		ctd->addedWord = &addedWord;
		ctd->playerTurnOverCond = &playerTurnOverCond;
		ctd->lock = &lock;
		ctd->connection_fd = client_fd;

		//Create the thread
		pthread_create(&new_tid, NULL, clientHandler, ctd);
	}

	return;
}

//The function each thread will attend clients on
void * clientHandler(void * arg) {
	cout << "Thread created\n";
	char buffer[BUFFER_SIZE];
	char bufferLong[BUFFER_LONG_SIZE];
	Scrabble * scrabble;
	bool * gameHasNewWord;
	bool * playerMadeMove;
	proposedWord_t * addedWord;
	pthread_cond_t * playerTurnOverCond;
	pthread_mutex_t * lock;
	int connection_fd;
	int playerID;

	//Get the game info
	clientThreadData * ctd = (clientThreadData *) arg;
	scrabble = ctd->scrabble;
	playerMadeMove = ctd->playerMadeMove;
	gameHasNewWord = ctd->gameHasNewWord;
	addedWord = ctd->addedWord;
	playerTurnOverCond = ctd->playerTurnOverCond;
	lock = ctd->lock;
	connection_fd = ctd->connection_fd;

	//Let the client know they have their own thread
	cout<<"Telling client about thread creation" << endl;
	sprintf(buffer, "OK");
	sendString(connection_fd, buffer, strlen(buffer)+1);

	//Get the player's name
	recvString(connection_fd, buffer, BUFFER_SIZE);
	cout << "Received client's name: " << buffer << endl;

	//Get the player ID for turn management later on
	playerID = scrabble->addPlayerToGame(buffer);
	int playerNumber = 0;

	// Ask client for number of players, if it is the first to connect
	if(playerID == 0){
		// It is the first client to connect
		sprintf(buffer, "SEND_PLAYER_NUMBER");
		sendString(connection_fd, buffer, strlen(buffer)+1);

		recvString(connection_fd, buffer, BUFFER_SIZE);
		sscanf(buffer, "%d", &playerNumber);
		scrabble->setSettingsPlayerNumber(playerNumber);

		cout << "Received players number " << playerNumber << endl;

	}else{
		// Wait for the first player to set gameSettings.playerNumber
		while (scrabble->getSettingsPlayerNumber() == 0){ }

		playerNumber = scrabble->getSettingsPlayerNumber();

		sprintf(buffer, "%d %d", playerNumber, playerID);
		sendString(connection_fd, buffer, strlen(buffer)+1);
		// Receives OK
		recvString(connection_fd, buffer, BUFFER_SIZE);

		if (playerID == playerNumber - 1) {
			scrabble->startGame();
			cout << "Game started" << endl;
		}

	}

	int turn = 0;

	// Wait until the game starts
	while(!scrabble->hasActiveGame){
	}

	// Check if it's spectator
	if (playerID >= playerNumber){
		// Spectator
		// Send words already in board
		sprintf(bufferLong, "%s", (char *)scrabble->getAddedWords().c_str());
		sendString(connection_fd, bufferLong, strlen(bufferLong)+1);
	}else{
		// Active player
		// Send hand
		sprintf(buffer, "%s", (char *)scrabble->getHand(playerID).c_str());
		sendString(connection_fd, buffer, strlen(buffer)+1);
	}

	// Receives OK
	recvString(connection_fd, buffer, BUFFER_SIZE);

	// Game Loop
	while(scrabble->hasActiveGame){
		// Calculate turn of player
		turn = scrabble->getTurn() % scrabble->getSettingsPlayerNumber();

		// Check if there are still minimum 2 players
		if(scrabble->countActivePlayers() < 2){
			scrabble->endGame();
		}

		while(!scrabble->isPlayerStillInGame(turn)){
			scrabble->nextTurn();
		}

		// TODO: read when client wants to leave (with signal) and call
		//scrabble->updateScoreboard();
		//scrabble->playerIsLeaving(playerID);

		if(turn == playerID) {
			*playerMadeMove = false;
			cout << "Turn of " << turn << endl;
			// turn of this thread player
			sprintf(buffer, "YOUR_TURN");
			sendString(connection_fd, buffer, strlen(buffer)+1);

			//Receive OK
			recvString(connection_fd, buffer, BUFFER_SIZE);

			//Send hand
			sprintf(buffer, "%s", (char *)scrabble->getHand(playerID).c_str());
			sendString(connection_fd, buffer, strlen(buffer)+1);

			//Receive OK
			recvString(connection_fd, buffer, BUFFER_SIZE);

			// Check what player wants to do
			if(!strcmp(buffer, "CHANGE")){
				// Wants to change tiles
				sprintf(buffer, "OK");
				sendString(connection_fd, buffer, strlen(buffer)+1);
				// Receives the tiles indexes separated by commas

				recvString(connection_fd, buffer, BUFFER_SIZE);
				printf("Player wants to exchange: %s\n", buffer);
				char indexes[BUFFER_SIZE];

				strcpy(indexes, buffer);

				// Split indexes string by commas to get each index
				char *string = strtok(indexes, ",");
				vector<int> tilesIndexes;
				while(string != NULL){
					tilesIndexes.push_back(atoi(string));
					string = strtok(NULL, ",");
				}
				// Change tiles
				scrabble->exchangeLetters(playerID, tilesIndexes);

				// Send hand
				sprintf(buffer, "%s", (char *)scrabble->getHand(playerID).c_str());
				sendString(connection_fd, buffer, strlen(buffer)+1);

				*gameHasNewWord = false;
			}else{
				char *word = new char[scrabble->getBoardSize() + 1];
				int x;
				int y;
				char direction;

				// Wants to add word
				sscanf(buffer, "%s %d %d %c", word, &x, &y, &direction);
				cout << "Player wants to add word: " << word << " at " << x << ", " << y << " in " << direction << " direction." << endl;

				string wordString(word);
				proposedWord_t proposedWord = proposedWord_t(wordString, x, y, direction);
				//Add word to game and replace the players' tiles if the word is valid
				int wordValue = scrabble->addWordToGame(proposedWord, playerID);
				while(wordValue < 0){
					cout << "Invalid!" << endl;
					// Invalid word
					// Send in protocol that it was not valid, try again
					sprintf(buffer, "INVALID");
					sendString(connection_fd, buffer, strlen(buffer)+1);

					recvString(connection_fd, buffer, BUFFER_SIZE);
					sscanf(buffer, "%s %d %d %c", word, &x, &y, &direction);
					cout << "Player wants to add word: " << word << " at " << x << ", " << y << " in " << direction << " direction." << endl;

					string wordString(word);
					proposedWord = proposedWord_t(wordString, x, y, direction);

					//Add word to game and replace the players' tiles if the word is valid
					wordValue = scrabble->addWordToGame(proposedWord, playerID);
				}

				cout << "Valid!" << endl;

				scrabble->printBoard();

				// Word added successfully send the value of the word added
				sprintf(buffer, "%d", wordValue);
				sendString(connection_fd, buffer, strlen(buffer)+1);

				*addedWord = proposedWord;
				*gameHasNewWord = true;

				// Check if game has ended
				if(scrabble->getHand(playerID) == ""){
					// Means that the player has used all its tiles and the letterpool is empty
					scrabble->endGame();
				}
			}

			//Start the mutex here so it affects all players
			pthread_mutex_lock(lock);
			pthread_cond_signal(playerTurnOverCond); 	//Signal the other threads to continue
			cout << "Sending turn over signal" << endl;
			//Unlock the mutex
			pthread_mutex_unlock(lock);
			
			*playerMadeMove = true;
			// Next turn in game
			scrabble->nextTurn();

		}else{
			// turn of other player
			sprintf(buffer, "%s", (char *)scrabble->getPlayerName(turn).c_str());
			sendString(connection_fd, buffer, strlen(buffer)+1);

			// Receives OK
			recvString(connection_fd, buffer, BUFFER_SIZE);

			// Wait until the turn player adds word to board
			//while(!*playerMadeMove){
			//}
			cout << "Waiting for turn over signal" << endl;
			pthread_cond_wait(playerTurnOverCond, lock);
			cout << "Received turn over signal" << endl;

			// Check what player did
			if(*gameHasNewWord) {
				// Send new word to client
				sprintf(buffer, "%s %d %d %c", (char *)addedWord->word.c_str(), addedWord->start.y, addedWord->start.x, addedWord->direction);
				sendString(connection_fd, buffer, strlen(buffer)+1);
			}else{
				// Player only changed tiles
				sprintf(buffer, "CHANGED");
				sendString(connection_fd, buffer, strlen(buffer)+1);
			}

		}

		// Receives OK
		recvString(connection_fd, buffer, BUFFER_SIZE);

	}

	// Game ended
	sprintf(buffer, "GAME_ENDED");
	sendString(connection_fd, buffer, strlen(buffer)+1);

	// Receives OK
	recvString(connection_fd, buffer, BUFFER_SIZE);

	// Send score, position and who won
	sprintf(buffer, "%s", (char *)scrabble->playerIsLeaving(playerID).c_str());
	sendString(connection_fd, buffer, strlen(buffer)+1);

	// Free arg memory
	free(ctd);

	pthread_exit(NULL);
}

// TODO: read ctrl+c signal in server and let the clients know, return their score and position
//scrabble->endGame();
// for(int i = 0; i < players.size(); i++)
		//scrabble->playerIsLeaving(i);
