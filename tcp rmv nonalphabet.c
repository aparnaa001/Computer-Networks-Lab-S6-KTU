// client


#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int clientsocket, port;
    struct sockaddr_in serveraddr;
    socklen_t len;
    char message[500];
    char cleaned[500];

    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;

    printf("Enter the port number: ");
    scanf("%d", &port);
    getchar(); // consume leftover newline
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    printf("\nTrying to connect to the server...\n");

    connect(clientsocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    printf("Connected to the server.\n");

    recv(clientsocket, message, sizeof(message), 0);
    printf("%s", message);

    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';

    send(clientsocket, message, sizeof(message), 0);

    recv(clientsocket, cleaned, sizeof(cleaned), 0);
    printf("Cleaned string (alphabets only): %s\n", cleaned);

    close(clientsocket);
    return 0;
}


//server


#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ctype.h>

void removeNonAlphabets(char *input, char *output);

int main()
{
    int serversocket, clientsocket, port;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len;
    char input[500], cleaned[500];

    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;

    printf("Enter the port number: ");
    scanf("%d", &port);

    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(serversocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(serversocket, 5);

    len = sizeof(clientaddr);
    printf("\nWaiting for client connection...\n");

    clientsocket = accept(serversocket, (struct sockaddr *)&clientaddr, &len);
    printf("Client connected.\n");

    write(clientsocket, "Enter a string: ", sizeof("Enter a string: "));
    read(clientsocket, input, sizeof(input));
    printf("Received from client: %s\n", input);

    removeNonAlphabets(input, cleaned);

    write(clientsocket, cleaned, sizeof(cleaned));

    close(clientsocket);
    close(serversocket);
    return 0;
}

void removeNonAlphabets(char *input, char *output)
{
    int i, j = 0;
    for (i = 0; input[i] != '\0'; i++)
    {
        if (isalpha(input[i]))
        {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
}
