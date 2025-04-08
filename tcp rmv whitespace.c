//server


// server.c

#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<unistd.h>

void replaceSpaces(char *input, char *output, char ch);

int main()
{
    int serversocket, clientsocket, port; 
    struct sockaddr_in serveraddr, clientaddr; 
    socklen_t len;  
    char input[500], modified[500]; 

    serversocket = socket(AF_INET, SOCK_STREAM, 0); 
    bzero((char*)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;

    printf("Enter the port number: ");
    scanf("%d", &port);

    serveraddr.sin_port = htons(port); 
    serveraddr.sin_addr.s_addr = INADDR_ANY; 

    bind(serversocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)); 

    len = sizeof(clientaddr); 
    listen(serversocket, 5); 

    printf("\nWaiting for client connection...\n"); 

    clientsocket = accept(serversocket, (struct sockaddr*)&clientaddr, &len);
    printf("Client connected.\n");

    // send prompt to client
    write(clientsocket, "Enter a string: ", sizeof("Enter a string: ")); 

    // receive string
    read(clientsocket, input, sizeof(input));
    printf("Received from client: %s\n", input);

    // process string
    replaceSpaces(input, modified, '#');

    // send back modified string
    write(clientsocket, modified, sizeof(modified));

    close(serversocket);
    close(clientsocket);
    return 0;
}

void replaceSpaces(char *input, char *output, char ch)
{
    int i, j = 0;
    for (i = 0; input[i] != '\0'; i++)
    {
        if (input[i] == ' ' || input[i] == '\t')
        {
            output[j++] = ch;
        }
        else
        {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
}



//client


// client.c

#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<strings.h>
#include<unistd.h>
#include<string.h>

int main()
{
    int clientsocket, port; 
    struct sockaddr_in serveraddr; 
    socklen_t len; 
    char message[500];
    char modified[500];

    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char*)&serveraddr, sizeof(serveraddr));
    len = sizeof(serveraddr); 
    serveraddr.sin_family = AF_INET; 

    printf("Enter the port number: ");
    scanf("%d", &port);
    getchar();  // clear newline left by scanf
    serveraddr.sin_port = htons(port);

    serveraddr.sin_addr.s_addr = INADDR_ANY;

    printf("\nTrying to connect to the server...\n");

    connect(clientsocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    printf("Connected to the server.\n");

    // receive prompt
    recv(clientsocket, message, sizeof(message), 0);
    printf("%s", message);

    // get input string
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0'; // remove newline

    // send to server
    send(clientsocket, message, sizeof(message), 0);

    // receive modified string
    recv(clientsocket, modified, sizeof(modified), 0);
    printf("Modified string: %s\n", modified);

    close(clientsocket);
    return 0;
}
