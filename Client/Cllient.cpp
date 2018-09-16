#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>

#include "IMP_Client.hpp"

int main(int argc, char *argv[])
{
//    IMP_Client* client = new IMP_Client("localhost", 51717);
    IMP_Client* client = new IMP_Client("192.168.0.22", 51717);
    
    client->initiate();
    
    std::cout << "Client running  Main Loop\n";
    unsigned int microseconds = 10000000;
    while(1) {
        usleep(microseconds);
        std::cout << "Client running < " << clock() << ">\n";
    }
    
    client->terminate();
    
#ifdef TEMPO
    char*   lArray = NULL;
    IMP_Message* lMessage = new IMP_Message(1, IMP_Message::eImageSize);
    lMessage->serialize(&lArray);
    
    IMP_Message* lMessage2 = new IMP_Message();
    lMessage2->unserialize(lArray);
#endif
#ifdef TEMPO
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    char buffer[256];
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    
    while (1) {
        printf("Please enter the message: ");
        bzero(buffer,256);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0)
            error("ERROR writing to socket");
    }
#endif
#ifdef COMMENTED
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0)
        error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);
#endif

    return 0;
}
