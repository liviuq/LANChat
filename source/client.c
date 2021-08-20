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
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    if((status = getaddrinfo(argv[1], PORT, &hints, &result)) != 0)
    {
        printf("[%s CLIENT] Error at getaddrinfo(): %s\n",__TIME__, gai_strerror(status));
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("[%s CLIENT] Finished getting the information\n", __TIME__);
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
    int n; //number of bytes written 

    while (1)
    {
        printf("Enter the message: ");
        memset(buffer, 0, 256);
        fgets(buffer, 255, stdin);
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
            printf("[%s CLIENT] Error on reading\n", __TIME__);
            exit(EXIT_FAILURE);
        }
        printf("=> %s", buffer);

    }
    
    //shuting down the connection to the server
    shutdown(socketfd, SHUT_RDWR);

    return 0;
}