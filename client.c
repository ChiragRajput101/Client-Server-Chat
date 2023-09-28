#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *error_msg) {
    perror(error_msg);
    exit(1);
} 

/*
    argv[] = { filename, server_ipaddr, portno_of_server }
*/

int main(int argc, char *argv[]) {
    if(argc < 3) error("Inadequate args");

    int socketFD;
    struct sockaddr_in server_addr;

    char buffer[255];

    int port_no = atoi(argv[2]);
    socketFD = socket(AF_INET, SOCK_STREAM, 0);

    if(socketFD < 0) error("Cant open the socket");

    struct hostent *server;
    server = gethostbyname(argv[1]);
    if(server == NULL) fprintf(stderr, "No such host");

    bzero((char *) &server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;

    bcopy((char *) server->h_addr_list[0], 
        (char *) &server_addr.sin_addr.s_addr, server->h_length);

    server_addr.sin_port = htons(port_no);

    int Connect = connect(socketFD, 
        (struct sockaddr *) &server_addr, sizeof(server_addr));

    if( Connect < 0 ) error("Connection Failed");

    /*Comm */

    while(1) {
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        int n = write(socketFD, buffer, strlen(buffer));

        if( n < 0 ) error("Error on writing the message");

        bzero(buffer, 255);

        n = read(socketFD, buffer, 255);
        if( n < 0 ) error("Error on read");

        printf("Server: %s", buffer);

        if( strncmp("quit", buffer, 4) == 0 ) break;
    }   

    close(socketFD);
    return 0;
}
