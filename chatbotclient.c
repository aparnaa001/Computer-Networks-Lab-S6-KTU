#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int sockdp;
    struct sockaddr_in server;
    socklen_t len=sizeof(server);
    char buffer[1024]={0};

    sockdp=socket(AF_INET, SOCK_STREAM,0);
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    server.sin_port=htons(PORT);

    connect(sockdp,(struct sockaddr*)&server,len);

    while(1)
{
    printf("Client: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';  // remove newline

    if(strcmp(buffer, "exit") == 0)
    {
        send(sockdp, buffer, strlen(buffer), 0);
        break;
    }

    send(sockdp, buffer, strlen(buffer), 0);

    memset(buffer, 0, sizeof(buffer));
    read(sockdp, buffer, sizeof(buffer));
    buffer[strcspn(buffer, "\n")] = '\0';
    printf("Server: %s\n", buffer);
}

    close(sockdp);
    
    return 0;
}
