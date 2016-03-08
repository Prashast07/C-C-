//
//  client.cpp
//  UDP
//
//  Created by Prashast  Kumar on 3/5/16.
//  Copyright © 2016 Prashast  Kumar. All rights reserved.
//

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg){
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
    int socketFileDescriptor, n;
    int length;
    struct sockaddr_in server, from;
    struct hostent *hp;
    char buffer[256];
    
    if (argc != 3) {
        printf("Usage: server port \n");
        exit(1);
    }
    socketFileDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFileDescriptor < 0) {
        error("ERROR : In Socket");
    }
    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    
    if (hp == 0) {
        error("Error: Unknown Host");
    }
    bcopy((char *)hp->h_addr,(char *)&server.sin_addr, hp->h_length);
    server.sin_port = htons(atoi(argv[2]));
    length = sizeof(struct sockaddr_in);
    printf("Please enter the message ::");
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    n = sendto(socketFileDescriptor, buffer, strlen(buffer), 0, (struct sockaddr *)&server, length);
    if(n < 0){
        error("ERROR: In Send to");
    }
    n = recvfrom(socketFileDescriptor, buffer, 256, 0,NULL,NULL);
    if(n < 0){
        error("ERROR: In Recieve From");
    }
    write(1, "Got An Acknowledgement:: \n", 24);
    write(1, buffer, n);
    
}
