/* ============================================================================
 * CS 8803 Introduction to Operating Systems
 * Project 1 Multi-Threaded Web Server
 *
 * Implements the server Project 1.
 *Zhihao Zou
============================================================================ */

#include <stdio.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/* CONSTANTS ===========================================================*/
#define SERVER_PORT 8888
#define BUFFER_SIZE 1024
/*======================================================================*/

int main(int argc, char **argv) {

	int socket_fd = 0; //server socket file description
	int client_socket_fd = 0; //client socket file description

	char buffer[BUFFER_SIZE]; //buffer for communication between server and client

	int num_bytes = 0; //number of bytes read from client

	int set_reuse_addr = 1;
	int max_pending_connections = 1;

	struct sockaddr_in server; //server socket address structure
	struct sockaddr_in client; //client socket address structure
	struct hostent *client_host_info; //client host information
	char *client_host_ip; //client host ip address
	socklen_t client_addr_len;

	// Create socket (IPv4, stream-based, protocol likely set to TCP)
	if (0 > (socket_fd = socket(AF_INET, SOCK_STREAM, 0))) {
		fprintf(stderr, "server failed to create the listening socket\n");
		exit(1);
	}

	// Set socket to use wildcards
	if (0 != setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &set_reuse_addr, sizeof(set_reuse_addr))) {
		fprintf(stderr, "server failed to set SO_REUSEADDR socket option (not fatal)\n");
	}

    // Configure server socket address structure (init to zero, IPv4, network byte
    //order for port and address)
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(SERVER_PORT);

    // Bind the socket
    if (0 > bind(socket_fd, (struct sockaddr *)&server, sizeof(server))) {
        fprintf(stderr, "server failed to bind\n");
        exit(1);
    }

    // Listen on the socket for up to some maximum pending connections
    if (0 > listen(socket_fd, max_pending_connections)) {
        fprintf(stderr, "server failed to listen\n");
        exit(1);
    }
    else {
        fprintf(stdout, "server listening for a connection on port %d\n", SERVER_PORT);
    }

    // Get the size client's address structure
    client_addr_len = sizeof(client);

    while(1) {
    		printf("\nWaiting for a connection\n");
    		// Accept a new client
		if (0 > (client_socket_fd = accept(socket_fd, (struct sockaddr *)&client, &client_addr_len))) {
			fprintf(stderr, "server accept failed\n");
		}
		else {
			fprintf(stdout, "\nServer accepted a client!\n");
		}

		printf("\nGot a connection!\n");

		 // Determine who sent the echo so that we can respond
		client_host_info = gethostbyaddr((const char *)&client.sin_addr.s_addr, sizeof(client.sin_addr.s_addr), AF_INET);
		if (client_host_info == NULL) {
			fprintf(stderr, "server could not determine client host address\n");
		}
		client_host_ip = inet_ntoa(client.sin_addr);
		if (client_host_ip == NULL) {
			fprintf(stderr, "server could not determine client host IP\n");
		}
		fprintf(stdout, "\nServer established connection with %s (%s)\n", client_host_info->h_name, client_host_ip);

		// Read request from the client and parse the request protocol
		bzero(buffer, BUFFER_SIZE);
		num_bytes = read(client_socket_fd, buffer, BUFFER_SIZE);
		if (num_bytes == 0) {
			fprintf(stderr, "server could not read from socket\n");
		} else {
			fprintf(stdout, "\nServer received %d bytes: %s\n", num_bytes, buffer);

			char *get_file_request_token = strtok(buffer, " ");
			 if (strcmp(get_file_request_token, "GetFile") == 0) {
				 get_file_request_token = strtok(NULL, " ");
				 if (strcmp(get_file_request_token, "GET") == 0) {
					 get_file_request_token = strtok(NULL, " ");
				 }
			 }
			 fprintf(stdout, "\nThe client is asking for this file: %s\n", get_file_request_token);
		}

		char response[] = "Hey, I received your request, and I am going to"
				" send you the file.\n";

		// Echo back to the client
		if (0 > write(client_socket_fd, response, strlen(response))) {
			fprintf(stderr, "server could not write back to socket\n");
		} else {
			fprintf(stdout, "\nServer sending message back to client\n");
		}

		printf("\nClosing the socket\n");
		//close client socket
		if(0 > close(client_socket_fd)) {
			printf("\nCould not close socket\n");
			return 0;
		}
    }
    // Close the socket and return
    close(socket_fd);
    return 0;
}