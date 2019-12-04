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

// Function declarations
void waitForConnections(int server_fd, Scrabble *scrabble, bool *playerMadeMove, bool *gameHasNewWord, proposedWord_t *addedWord);
void * clientHandler(void * arg);
void usage(char * program);

//The struct to be sent to the thread
typedef struct clientThreadDataStruct {
	Scrabble * scrabble;
	// Word that was just successfully added
	bool * playerMadeMove;
	bool * gameHasNewWord;
	proposedWord_t * addedWord;

	int connection_fd;
} clientThreadData;

int main(int argc, char * argv[]) {
	// Check the correct arguments
	if (argc != 2)
	{
			usage(argv[0]);
	}

	Scrabble scrabble = Scrabble();

	// TODO: ask this to the first player
	scrabble.setDictionary("dictionaries/english.txt");

	// Set up variables for thread data
	bool playerMadeMove;
	bool gameHasNewWord;
	proposedWord_t addedWord;

	//Set up the sever
	int server_fd;
	server_fd = initServer(argv[1], MAX_CLIENTS);

	// Listen for connections from the clients
	waitForConnections(server_fd, &scrabble, &playerMadeMove, &gameHasNewWord, &addedWord);

	return 1;
}

void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}


//Server is running, waiting for player to start game
void waitForConnections(int server_fd, Scrabble *scrabble, bool *playerMadeMove, bool *gameHasNewWord, proposedWord_t *addedWord)
{
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
			ctd->playerMadeMove = playerMadeMove;
			ctd->gameHasNewWord = gameHasNewWord;
			ctd->addedWord = addedWord;
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
	Scrabble * scrabble;
	bool * gameHasNewWord;
	bool * playerMadeMove;
	proposedWord_t * addedWord;
	int connection_fd;
	int playerID;

	//Get the game info
	clientThreadData * ctd = (clientThreadData *) arg;
	scrabble = ctd->scrabble;
	playerMadeMove = ctd->playerMadeMove;
	gameHasNewWord = ctd->gameHasNewWord;
	addedWord = ctd->addedWord;
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

	// Ask client for number of players, if it is the first to connect
	if(playerID == 0){
		// It is the first client to connect
		sprintf(buffer, "SEND_PLAYER_NUMBER");
		sendString(connection_fd, buffer, strlen(buffer)+1);

		int playerNumber = 0;

		recvString(connection_fd, buffer, BUFFER_SIZE);
		sscanf(buffer, "%d", &playerNumber);
		scrabble->setSettingsPlayerNumber(playerNumber);

		cout << "Received players number " << playerNumber << endl;

	}else{
		// Wait for the first player to set gameSettings.playerNumber
		while (scrabble->getSettingsPlayerNumber() == 0){ }

		sprintf(buffer, "%d %d", scrabble->getSettingsPlayerNumber(), playerID);
		sendString(connection_fd, buffer, strlen(buffer)+1);
		// Receives OK
		recvString(connection_fd, buffer, BUFFER_SIZE);

		if (playerID == scrabble->getSettingsPlayerNumber() - 1) {
			scrabble->startGame();
			cout << "Game started" << endl;
		}
		
	}

	int turn = 0;
	
	// Wait until the game starts
	while(!scrabble->hasActiveGame){
	}

	// Send hand
	sprintf(buffer, "%s", (char *)scrabble->getHand(playerID).c_str());
	sendString(connection_fd, buffer, strlen(buffer)+1);

	printf("%s\n", buffer);

	// Receives OK
	recvString(connection_fd, buffer, BUFFER_SIZE);

	// Game Loop
	while(scrabble->hasActiveGame){
		// Calculate turn of player
		turn = scrabble->getTurn() % scrabble->getSettingsPlayerNumber();

		if(turn == playerID) {
			cout << "Turn of " << turn << endl;
			// turn of this thread player
			sprintf(buffer, "YOUR_TURN");
			sendString(connection_fd, buffer, strlen(buffer)+1);

			recvString(connection_fd, buffer, BUFFER_SIZE);

			// Check what player wants to do
			if(!strcmp(buffer, "CHANGE")){
				// Wants to change tiles
				sprintf(buffer, "OK");
				sendString(connection_fd, buffer, strlen(buffer)+1);
				// Receives the tiles indexes separated by commas
				string indexes;
				recvString(connection_fd, buffer, BUFFER_SIZE);

				sscanf(buffer, "%s", (char *)indexes.c_str());
				// Split indexes string by commas to get each index
				std::stringstream ss(indexes);
				vector<int> tilesIndexes;
				for (int i; ss >> i;) {
						tilesIndexes.push_back(i);    
						if (ss.peek() == ',')
							ss.ignore();
				}
				// Change tiles
				scrabble->exchangeLetters(playerID, tilesIndexes);

				// Send hand
				sprintf(buffer, "%s", (char *)scrabble->getHand(playerID).c_str());
				sendString(connection_fd, buffer, strlen(buffer)+1);

				*playerMadeMove = true;
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

					wordValue = scrabble->addWordToGame(proposedWord, playerID);
				}

				cout << "Valid!" << endl;

				scrabble->printBoard();

				// Word added successfully send the value of the word added
				sprintf(buffer, "%d", wordValue);
				sendString(connection_fd, buffer, strlen(buffer)+1);

				*addedWord = proposedWord;
				*gameHasNewWord = true;
				*playerMadeMove = true;
			}

			// Next turn in game
			scrabble->nextTurn();

		}else{
			// turn of other player
			sprintf(buffer, "%s", (char *)scrabble->getPlayerName(turn).c_str());
			sendString(connection_fd, buffer, strlen(buffer)+1);

			// Receives OK
			recvString(connection_fd, buffer, BUFFER_SIZE);

			// Wait until the turn player adds word to board
			while(!*playerMadeMove){
			}

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
		*playerMadeMove = false;

		// Receives OK
		recvString(connection_fd, buffer, BUFFER_SIZE);

	}


	// Free arg memory
	free(ctd);

	pthread_exit(NULL);
}
