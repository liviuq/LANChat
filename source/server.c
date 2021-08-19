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
    //checking for arguments
    if (argc != 1)
    {
        printf("Usage: %s\n", *argv);
        exit(EXIT_FAILURE);
    }

    int status;                 //return value of getaddrinfo()
    struct addrinfo hints;      //setup the basic socket settings
    struct addrinfo *serverinfo;//here we have the hints, IP and port

    //setting up structures
    memset(&hints, 0, sizeof(struct addrinfo));     //mandatory to fill it with 0
    hints.ai_family = AF_INET;                      //using IPv4
    hints.ai_socktype = SOCK_STREAM;                //TCP socket
    hints.ai_flags = AI_PASSIVE;                    //intended for server to listen on localhost,
                                                    //instead of hardcoding it

    //passing NULL in combination with AI_PASSIVE
    if((status = getaddrinfo(NULL, PORT, &hints, &serverinfo)) != 0)
    {
        printf("[%s SERVER] Error at getaddressinfo(): %s\n",__TIME__, gai_strerror(status));
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("[%s SERVER] Finished getting the information\n", __TIME__);
    }

    int socketfd;        //server socket, this will receive incoming connections
    if((socketfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol)) == -1)
    //domain -> AF_INET which means IPv4 socket
    //type -> SOCK_STREAM which means TCP socket
    //protocol -> usually 0
    {
        printf("[%s SERVER] Error at socket syscall\n", __TIME__);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("[%s SERVER] Finished creating socket\n", __TIME__);
    }

    //bind the server socket to the IP and port
    //ai_addr has them both
    if(bind(socketfd, serverinfo->ai_addr, serverinfo->ai_addrlen) == -1)
    {
       printf("[%s SERVER] Error on binding\n", __TIME__);
       exit(EXIT_FAILURE);
    }
    else
    {
        printf("[%s SERVER] Binding successfull\n", __TIME__);
    }

    //listening to the incoming connections
    //if there are more than 10 connection requests in the queue,
    //drop the remaining ones
    if (listen(socketfd, BACKLOG) < 0)
    {
        printf("[%s SERVER] Error on listening\n", __TIME__); 
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("[%s SERVER] Listening on port %d\n", __TIME__, atoi(PORT));
    }

    int clientsocketfd;                         //file descriptor to talk strictly to the new client
    struct sockaddr_in clientaddress;           //client's IP and port
    socklen_t clientlength;                     //length of clientaddress
    clientlength= sizeof(struct sockaddr_in);
    //accepts the first connection in the queue
    //struct sockaddr_in can be safely casted to struct sockaddr
    clientsocketfd = accept(socketfd, (struct sockaddr *)&clientaddress, &clientlength);
    if (clientsocketfd < 0)
    {
        printf("[%s SERVER] Error on accepting\n", __TIME__);
        exit(EXIT_FAILURE);
    }
    else
        printf("[%s SERVER] Accepted incoming connection\n", __TIME__);

    //freeing the memory
    freeaddrinfo(serverinfo);

    //storage for incoming data
    char *buffer = malloc(256 * sizeof(char));
    int n;

    //read while there is stuff to read
    while (1)
    {
        memset(buffer, 0, 256);
        if (read(clientsocketfd, buffer, 256) <= 0)
        {
            printf("[%s SERVER] Error on reading\n", __TIME__);
            exit(EXIT_FAILURE);
        }
        printf("=> %s", buffer);

        printf("Enter the message: ");
        memset(buffer, 0, 256);
        fgets(buffer, 255, stdin);
        if ((n = write(clientsocketfd, buffer, strlen(buffer))) <= 0)
        {
            printf("[SERVER] Error writing to socket\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("[SERVER] Written %d bytes\n", n);
        }
    }
    return 0;
}