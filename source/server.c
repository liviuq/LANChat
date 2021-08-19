#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT "5050"
#define BACKLOG 10

int main(int argc, char *argv[])
{
    if (argc != 1)
    {
        printf("Usage: %s\n", *argv);
        exit(EXIT_FAILURE);
    }

    int status;
    struct addrinfo hints;
    struct addrinfo *serverinfo;

    //setting up structures
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    //pass NULL for getaddrinfo() for localhost listening(later)
    if((status = getaddrinfo(NULL, PORT, &hints, &serverinfo)) != 0)
    {
        printf("[%s SERVER] Error at getaddressinfo(): %s\n",__TIME__, gai_strerror(status));
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("[%s SERVER] Finished getting the information\n", __TIME__);
    }

    int socketfd;
    if((socketfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol)) == -1)
    {
        printf("[%s SERVER] Error at socket syscall\n", __TIME__);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("[%s SERVER] Finished creating socket\n", __TIME__);
    }

    if(bind(socketfd, serverinfo->ai_addr, serverinfo->ai_addrlen) == -1)
    {
       printf("[%s SERVER] Error on binding\n", __TIME__);

       exit(EXIT_FAILURE);
    }
    else
    {
        printf("[%s SERVER] Binding successfull\n", __TIME__);
    }

    if (listen(socketfd, 5) < 0)
    {
        printf("[%s SERVER] Error on listening\n", __TIME__); 
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("[%s SERVER] Listening on port %d\n", __TIME__, atoi(PORT));
    }

    int clientsocketfd;
    struct sockaddr_in clientaddress;
    socklen_t clientlength;
    clientlength= sizeof(struct sockaddr_in);
    clientsocketfd = accept(socketfd, (struct sockaddr *)&clientaddress, &clientlength);
    if (clientsocketfd < 0)
    {
        printf("[%s SERVER] Error on accepting\n", __TIME__);
        exit(EXIT_FAILURE);
    }
    else
        printf("[%s SERVER] Accepted incoming connection\n", __TIME__);

    freeaddrinfo(serverinfo);

    char *buffer = malloc(256 * sizeof(char));

    while (1)
    {
        memset(buffer, 0, 256);
        if (read(clientsocketfd, buffer, 256) < 0)
        {
            printf("[%s SERVER] Error on reading\n", __TIME__);
            exit(EXIT_FAILURE);
        }

        printf("The message is: %s\n", buffer);
    }
    return 0;
}