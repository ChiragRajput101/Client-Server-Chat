#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *error_msg) {
    perror(error_msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if(argc == 1) {
        fprintf(stderr, "port number isnt provided!\n");
        exit(1);
    }

    int socketFD, connSocketFD; // file descriptors for identification
    char buffer[255]; // message buffer

    // Structure describing an Internet socket address
    struct sockaddr_in server_addr, client_addr; 
    socklen_t client_length;  

    // AF_INET: IPv4, SOCK_STREAM: TCP Stream
    socketFD = socket(AF_INET, SOCK_STREAM, 0); 
    // successful conn -> 0

    if(socketFD != 0) {
        error("Socket couldn't be opened");
    }

    bzero((char *) &server_addr, sizeof(server_addr));

    int port_no = atoi(argv[1]); // from CL

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_no);

    // sockaddr -> defines a generic socket addr 
    if( bind(socketFD, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0 ) {
        error("Binding Failed");
    }

    
    return 0;
}


