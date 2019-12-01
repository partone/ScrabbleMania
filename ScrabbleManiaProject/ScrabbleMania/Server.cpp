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


#define HAND_SIZE 7
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

// Function declarations
void playersLobby(Scrabble *scrabble);
void game(Scrabble *scrabble);
void waitForConnections(int server_fd, Scrabble *scrabble);
void * clientHandler(void * arg);
void usage(char * program);

int main(int argc, char * argv[]) {
	// Check the correct arguments
	if (argc != 2)
	{
			usage(argv[0]);
	}

	Scrabble scrabble = Scrabble();
	scrabble.setPlayerNumber(strtol(argv[2], NULL, 10));

	//Set up the sever
	int server_fd;
	server_fd = initServer(argv[1], MAX_CLIENTS);

	// Listen for connections from the clients
	waitForConnections(server_fd, &scrabble);

	//playersLobby(&scrabble);

	//game(&scrabble);

	return 1;
}

void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {port_number} {player_number}\n", program);
    exit(EXIT_FAILURE);
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
		scrabble->printScores();
		if (i == 10) {
			scrabble->endGame();
		}
		i++;
	}

	// scrabble->printBoard();
	// scrabble->endGame();

	// Finish the main thread
	pthread_exit(NULL);
}


//Server is running, waiting for player to start game
void waitForConnections(int server_fd, Scrabble *scrabble)
{
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int client_fd;
    pthread_t new_tid;
		clientThreadData * ctd = NULL;		//For the data sent to the thread

		//Allow each player to connect to the game
    for(int i = 0; i < scrabble->getPlayerNumber(); i++) {
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
				ctd->connection_fd = client_fd;

				//Create the thread
	      pthread_create(&new_tid, NULL, clientHandler, ctd);
		}
		//TODO: Figure out how to manage the game
		cout << "All client connected!" << endl;

    return;
}

//The function each thread will attend clients on
void * clientHandler(void * arg) {
	cout << "Thread created\n";
	char buffer[BUFFER_SIZE];
	Scrabble * scrabble;
	int connection_fd;
	int playerID;

	//Get the game info
	clientThreadData * ctd = (clientThreadData *) arg;
	scrabble = ctd->scrabble;
	connection_fd = ctd->connection_fd;

	//Let the client know they have their own thread
	sscanf(buffer, "OK");
	cout<<"Telling client about thread creation";
	sendString(connection_fd, buffer, 3);

	//Get the player's name
	recvString(connection_fd, buffer, BUFFER_SIZE);
	cout << "Received client's name: " << buffer << endl;

	//Get the player ID for turn management later on
	playerID = scrabble->addPlayerToGame(buffer);




	pthread_exit(NULL);
}
