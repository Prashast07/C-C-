//
//  client.cpp
//  TCP
//
//  Created by Prashast  Kumar on 3/4/16.
//  Copyright © 2016 Prashast  Kumar. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int socketFiledescriptor, portNumber, n;
    struct sockaddr_in serverAddress; // Contains the address of the server we want to connect
    struct hostent *server; //Defines a host in the Internet
    
    char buffer[256];
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portNumber = atoi(argv[2]);
    
    socketFiledescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFiledescriptor < 0){
         error("ERROR opening socket");
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serverAddress.sin_addr.s_addr,server->h_length);
    serverAddress.sin_port = htons(portNumber);
    if (connect(socketFiledescriptor,(struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0)
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(socketFiledescriptor,buffer,strlen(buffer));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(socketFiledescriptor,buffer,255);
    if (n < 0)
        error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(socketFiledescriptor);
    return 0;
}
