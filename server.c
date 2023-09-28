#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_ClIENT_REQ 5

void error(const char *error_msg) {
    perror(error_msg);
    exit(1);
}

/*
    argv[] = { filename, portno_of_server }
    ( filename -- executable's name ) 
*/

int main(int argc, char *argv[]) {
    if(argc < 1) {
        fprintf(stderr, "port number isnt provided!\n");
        exit(1);
    }

    int socketFD, connSocketFD; // file descriptors for identification
    char buffer[255]; // message buffer

    // Structure describing an Internet socket address
    struct sockaddr_in server_addr, client_addr; 

    // AF_INET: IPv4, SOCK_STREAM: TCP Stream
    socketFD = socket(AF_INET, SOCK_STREAM, 0); 
    // successful conn -> 0

    if(socketFD < 0) {
        error("Socket couldn't be opened");
    }

    bzero((char *) &server_addr, sizeof(server_addr));

    int port_no = atoi(argv[1]); // from CL

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_no);

    // sockaddr -> defines a generic socket addr 
    int Bind = bind(socketFD, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if( Bind < 0 ) {
        error("Binding Failed");
    }

    listen(socketFD, MAX_ClIENT_REQ);

    socklen_t client_length; 
    client_length = sizeof(client_addr);

    connSocketFD = accept(socketFD, (struct sockaddr *) &client_addr, &client_length);

    if(connSocketFD < 0) {
        error("Connect Failed, Not Accepted");
    }

    /*Communication */

    while(1) {
        bzero(buffer, 255);
        int n = read(connSocketFD, buffer, 255);
        if(n < 0) {
            error("Error on read");
        }

        printf("Client: %s\n", buffer);
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);

        n = write(connSocketFD, buffer, strlen(buffer));
        if(n < 0) error("Wasn't able to write the message to FD");

        int terminate = strncmp("quit", buffer, 4);
        if(terminate == 0) break;
    }

    close(connSocketFD);
    close(socketFD);

    return 0;
}


