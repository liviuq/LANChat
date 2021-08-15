#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int
main(int argc, char* argv[])
{
    int sockfd, newsockfd, portno, clientlen, n;
    char *buffer = malloc(sizeof(char) * 256);

    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2)
    {
        printf("Usage: %s <port>\n",*argv);
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        printf("[SERVER] Error opening socket\n");
        exit(EXIT_FAILURE);
    } 

    memset(&serv_addr, 0, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if( bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("[SERVER] Error on binding\n");
        exit(EXIT_FAILURE);
    }

    if(listen(sockfd, 5) < 0)
    {
        printf("[SERVER] Error on listening\n");
        exit(EXIT_FAILURE);
    }

    clientlen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clientlen);
    if(newsockfd <0)
    {
        printf("[SERVER] Error on accepting\n");
        exit(EXIT_FAILURE);
    }

    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);
    if(n < 0)
    {
        printf("[SERVER] Error on reading\n");
        exit(EXIT_FAILURE);
    }
    else printf("The message is: %s\n", buffer);

    return 0;
}