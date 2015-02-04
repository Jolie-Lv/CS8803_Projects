/* ============================================================================
 * CS 8803 Introduction to Operating Systems
 * Project 1 Multi-Threaded Web Server
 *
 * Implements the clent Project 1.
 *Zhihao Zou
============================================================================ */

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* CONSTANTS =============================================================== */
#define SERVER_ADDR "localhost"
#define SERVER_PORT 8888
#define BUFFER_SIZE 1024
#define FILE_NAME "/home/adminuser/Documents/CS8803/Project1/skeleton/lastname-firstname-pr1/1kb-sample-file-2.png"
//=============================================================================

//The GetFile Protocol
const char * get_file_request(char *filename) {
	char get_file_content[250] = "GetFile GET ";
	printf("The header is: %s\n", get_file_content);
	return strcat(get_file_content, filename);
}

int main(int argc, char **argv) {

    int socket_fd = 0;
    struct sockaddr_in server_socket_addr;
    char buffer[BUFFER_SIZE];

    // Converts localhost into 0.0.0.0
    struct hostent *he = gethostbyname(SERVER_ADDR);
    unsigned long server_addr_nbo = *(unsigned long *)(he->h_addr_list[0]);

    // Create socket (IPv4, stream-based, protocol likely set to TCP)
    if (0 > (socket_fd = socket(AF_INET, SOCK_STREAM, 0))) {
        fprintf(stderr, "client failed to create socket\n");
        exit(1);
    }

    // Configure server socket address structure (init to zero, IPv4, network byte
    //order for port and address)
    bzero(&server_socket_addr, sizeof(server_socket_addr));
    server_socket_addr.sin_family = AF_INET;
    server_socket_addr.sin_port = htons(SERVER_PORT);
    server_socket_addr.sin_addr.s_addr = server_addr_nbo;

    // Connect socket to server
    if (0 > connect(socket_fd, (struct sockaddr *)&server_socket_addr, sizeof(server_socket_addr))) {
        fprintf(stderr, "client failed to connect to %s:%d!\n", SERVER_ADDR, SERVER_PORT);
        close(socket_fd);
        exit(1);
    }
    else {
        fprintf(stdout, "client connected to to %s:%d!\n", SERVER_ADDR, SERVER_PORT);
    }

    //construct a GetFile protocolcharasdfasdf
    const char *file_request = get_file_request("/home/adminuser/Documents/CS8803/Project1/skeleton/lastname-firstname-pr1/1kb-sample-file-2.png");

    fprintf(stdout, "The GetFile Request is: %s", file_request);

    // Send file request
    if (0 > send(socket_fd, file_request, strlen(file_request), 0)) {
        fprintf(stderr, "client failed to send echo message");
        close(socket_fd);
        exit(1);
    }

    // Process response from server
    bzero(buffer, BUFFER_SIZE);
    if(0 > read(socket_fd, buffer, BUFFER_SIZE)) {
        fprintf(stderr, "client could not read response from server\n");
        close(socket_fd);
        exit(1);
    } else {
        fprintf(stdout, "echo from server: %s\n", buffer);
    }

    // Close the socket and return the response length (in bytes)
    close(socket_fd);
    return 0;
}