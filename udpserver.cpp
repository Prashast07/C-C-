//
//  main.cpp
//  UDP
//
//  Created by Prashast  Kumar on 3/5/16.
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

void error(char *msg){
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
    int sockFileDescriptor,n;
    socklen_t length, fromLength;
    struct sockaddr_in server;
    struct sockaddr_in from;
    char buffer[1024];
    if(argc < 2){
        fprintf(stderr, "Error :: No Port Provided");
        exit(0);
    }
    sockFileDescriptor = socket(AF_INET, SOCK_DGRAM,0);
    if (sockFileDescriptor < 0) {
        error("Error :: Opening Socket");
    }
    length = sizeof(server);
    bzero(&server,length);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));
    if(bind(sockFileDescriptor,(struct sockaddr *)&server,length) < 0){
        error("Error:: In Binding");
    }
    fromLength = sizeof(struct sockaddr_in);
    while(1){
        n = recvfrom(sockFileDescriptor,buffer,1024,0,(struct sockaddr *)&from, & fromLength);
        if (n < 0) {
            error("Error:: recieve from");
        }
        write(1,"Recieved a Datagram:",21);
        write(1,buffer,n);
        n = sendto(sockFileDescriptor,"Got Your Message \n",17,0, (struct sockaddr *)&from,fromLength);
        if(n < 0){
            error("ERROR :: In Send to ");
        }
    }
}