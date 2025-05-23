//client



#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<strings.h>
#include<unistd.h>
int main()
{
    int clientsocket,port; 
    struct sockaddr_in serveraddr; 
    socklen_t len; 
    char message[50];

    int result;

    int number;


    clientsocket=socket(AF_INET,SOCK_STREAM,0);
    bzero((char*)&serveraddr,sizeof(serveraddr));
    len=sizeof(serveraddr); 
    serveraddr.sin_family=AF_INET; 

    printf("Enter the port number ");
    scanf("%d",&port);
    serveraddr.sin_port=htons(port);
    printf("\nTrying to connect to the server.\n");


    connect(clientsocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    printf("\nConnected to the server.\n");

    recv(clientsocket,message,sizeof(message),0);
    printf("\n%s ",message);

    scanf("%d",&number);
    send(clientsocket,&number,sizeof(number),0);

    recv(clientsocket,&result,sizeof(result),0);

    printf("\nResult is %d ",result);


    close(clientsocket);
}



//server



#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netdb.h>
#include<unistd.h>

int reverse(int n);

int main()
{
    int serversocket,clientsocket,port; 
    struct sockaddr_in serveraddr,clientaddr; 
    socklen_t len;  
    char message[50]; 
    struct serveraddr; 

    int numrecieved,reversenum;

    serversocket=socket(AF_INET,SOCK_STREAM,0); 
    bzero((char*)&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;

    printf("Enter the port number ");
    scanf("%d",&port);

    serveraddr.sin_port=htons(port); 
    serveraddr.sin_addr.s_addr=INADDR_ANY; 
    bind(serversocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr)); 
    bzero((char*)&clientaddr,sizeof(clientaddr)); 
    len=sizeof(clientaddr); 
    listen(serversocket,5); 

    printf("\nWaiting for client connection\n"); 
    printf("\nhai:");
    clientsocket=accept(serversocket,(struct sockaddr*)&clientaddr,&len);//accepting the client connection

    printf("\nClient connectivity received.\n");
    
    write(clientsocket,"Enter an integer:",sizeof("Enter an integer:")); 
    read(clientsocket,&numrecieved,sizeof(numrecieved));
    printf("\nNumber send from server : %d",numrecieved);

    reversenum = reverse(numrecieved);

    //send the reversed number to the client
    write(clientsocket,&reversenum,sizeof(reversenum));


    

    close(clientsocket);
    close(serversocket);

}

int reverse(int n)
{
    int rev=0;
    while(n!=0)
    {
        rev=rev*10;
        rev=rev+n%10;
        n=n/10;
    }
    return rev;
}
