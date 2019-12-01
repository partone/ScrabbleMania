
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>
// Custom libraries
#include "sockets.h"
#include "fatal_error.h"

using namespace std;

#define BUFFER_SIZE 1024


void usage(char * program);
void playGame(int connection_fd);

int main(int argc, char * argv[]) {
	int connection_fd;

	printf("\n=== Welcome to Scrabble ===\n");

	// Check the correct arguments
	if (argc != 3)
	{
			usage(argv[0]);
	}

	// Connect to the server
	connection_fd = connectSocket(argv[1], argv[2]);
	playGame(connection_fd);

	// Close the socket
	close(connection_fd);

	return 0;
}

void playGame(int connection_fd) {
	char buffer[BUFFER_SIZE];
	cout << "Connection successful, creating thread\n";
	//Wait for thread confirmation
	recvString(connection_fd, buffer, BUFFER_SIZE);
	cout << "Thread created\n";
	//Ask user for name
	cout << "What is your name?\n";
	cin >> buffer;
	sendString(connection_fd, buffer, strlen(buffer)+1);
	//Wait for confirmation that the other players are connected
	cout << "Waiting for other players...\n";

	//Let the player know that all clients have connected and the game will start
	recvString(connection_fd, buffer, BUFFER_SIZE);
	cout << "All players connected, game starting\n";


	//TODO: Game loop w/ turns and stuff
}

void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}
