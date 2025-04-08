#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int sockfd,newsock;
    struct sockaddr_in server,client;
    socklen_t len=sizeof(client);
    char buffer[1024]={0};

    sockfd=socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family= AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(PORT);

    bind(sockfd, (struct sockaddr*)&server, sizeof(server));
    listen(sockfd, 5);

    newsock=accept(sockfd, (struct sockaddr*)&client, &len);

    while(1)
{
    memset(buffer, 0, sizeof(buffer));
    read(newsock, buffer, sizeof(buffer));
    buffer[strcspn(buffer, "\n")] = '\0';  // remove newline
    printf("Client: %s\n", buffer);

    if(strcmp(buffer, "exit") == 0)
        break;

    printf("Server: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    send(newsock, buffer, strlen(buffer), 0);
}

    close(sockfd);
    close(newsock);
    return 0;
}
