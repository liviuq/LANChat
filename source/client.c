#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT "5050"

int main(int argc, char *argv[])
{
    //checking for arguments
    if (argc != 2)
    {
        printf("Usage: %s <hostname>\n", *argv);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("[CLIENT] Parsed input parameters\n");
    }

    int status;                 //return value of getaddrinfo()
    struct addrinfo hints;      //setup the basic socket settings
    struct addrinfo *result;    //here we have the hints, IP and port
    memset(&hints, 0, sizeof(struct addrinfo)); //memset to 0 as described in the documentation
                                                //the function bzero is obsolete, referenced in addrinfo
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    if((status = getaddrinfo(argv[1], PORT, &hints, &result)) != 0)
    {
        printf("[CLIENT] Error at getaddrinfo(): %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("[CLIENT] Finished getting the information\n");
    }

    int socketfd;       //server socket, this will receive incoming connections
    socketfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    //domain -> AF_INET which means IPv4 socket
    //type -> SOCK_STREAM which means TCP socket
    //protocol -> usually 0
    if (socketfd < 0)
    {
        printf("[CLIENT] Error opening socket\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("[CLIENT] Opened socket on port %d\n", atoi(PORT));
    }

    //connecting to the server
    //we should iterate through the linked list ~result~ so we can pick a valid entry
    //as the first entry might be invalid.
    if (connect(socketfd, result->ai_addr, result->ai_addrlen) < 0)
    {
        printf("[CLIENT] Error connecting\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("[CLIENT] Connected to socket\n");
    }

    //freeing the linked list
    freeaddrinfo(result);

    //storage for incoming data
    char *buffer = malloc(256 * sizeof(char));
    //number of bytes written 
    int n;

    //loop while you want to exchange messages
    for(;;) //as in the example from getaddrinfo(3)
    {
        printf("Enter the message: ");
        memset(buffer, 0, 256);
        char* ret;
        if( (ret = fgets(buffer, 255, stdin)) == NULL)
        {
            printf("[CLIENT] Error connecting\n");
            exit(EXIT_FAILURE);
        }

        if ((n = write(socketfd, buffer, strlen(buffer))) <= 0)
        {
            printf("[CLIENT] Error writing to socket\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("[CLIENT] Written %d bytes\n", n);
        }

        if (read(socketfd, buffer, 256) <= 0)
        {
            printf("[CLIENT] Error on reading\n");
            exit(EXIT_FAILURE);
        }
        printf("=> %s", buffer);

    }
    
    //shuting down the connection to the server
    shutdown(socketfd, SHUT_RDWR);

    return 0;
}