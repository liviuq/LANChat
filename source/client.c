#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int
main(int argc, char* argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char *buffer = malloc(sizeof(char) * 256);
    bzero(buffer, 256);
    if(argc < 3)
    {
        printf("Usage: %s <hostname> <port>\n",*argv);
        exit(EXIT_FAILURE);
    }
    else 
        printf("[CLIENT] Parsed input parameters\n");
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        printf("[CLIENT] Error opening socket\n");
        exit(EXIT_FAILURE);
    } 
    else 
        printf("[CLIENT] Opened socket on port %d\n", portno);

    server = gethostbyname(argv[1]);
    if(server == NULL)
    {
        printf("[CLIENT] No such host\n");
        exit(EXIT_FAILURE);
    } 
    else 
        printf("[CLIENT] Trying to connect to %s\n", server->h_name);

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if(connect(sockfd,(const struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("[CLIENT] Error connecting\n");
        exit(EXIT_FAILURE);
    } 
    else 
        printf("[CLIENT] Connected to socket\n");

    printf("Enter the message: ");
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    n = write(sockfd, buffer, strlen(buffer));
    if(n < 0)
    {
        printf("[CLIENT] Error writing to socket\n");
        exit(EXIT_FAILURE);
    } 
    else 
        printf("[CLIENT] Written %d bytes\n", n);

    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if(n < 0)
    {
        printf("[CLIENT] Error reading from socket\n");
        exit(EXIT_FAILURE);
    } 
    else 
        printf("[CLIENT] Read %d bytes\n", n);

    printf("%s\n", buffer);

    return 0;
}