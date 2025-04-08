//client

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<netdb.h>
#include<strings.h>

int main() {
    int number; // Variable to store the number
    int clientsocket, port; // Socket descriptor and port number
    struct sockaddr_in serveraddr; // Server address structure
    socklen_t len; // Variable to store the length of server address
    char message[100]; // Char array to store the response message

    clientsocket = socket(AF_INET, SOCK_DGRAM, 0); // Create a UDP socket

    bzero((char*)&serveraddr, sizeof(serveraddr)); // Initialize server address to zero
    len = sizeof(serveraddr); // Store the length of the server address

    serveraddr.sin_family = AF_INET; // Set address family to AF_INET

    // Get port number from the user
    printf("Enter the port number: ");
    scanf("%d", &port);
    serveraddr.sin_port = htons(port); // Convert port to network byte order

    // Get number to be converted into words
    printf("Enter a number to convert to words: ");
    scanf("%d", &number);

    // Send the number to the server
    sendto(clientsocket, &number, sizeof(number), 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    // Receive the result from the server
    recvfrom(clientsocket, message, sizeof(message), 0, (struct sockaddr*)&serveraddr, &len);
    printf("Server Response: %s\n", message); // Display the number in words

    close(clientsocket); // Close the socket
}


//server



#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<strings.h>

// Function to convert a number into words
void number_to_words(int num, char *result) {
    // Arrays of words for numbers
    const char *ones[] = {"", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    const char *teens[] = {"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
    const char *tens[] = {"", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};

    if (num == 0) {
        strcpy(result, "zero");
        return;
    }

    if (num >= 100) {
        int hundreds = num / 100;
        num %= 100;
        sprintf(result, "%s hundred", ones[hundreds]);
        if (num > 0) {
            strcat(result, " and ");
        }
    }

    if (num >= 20) {
        int ten_part = num / 10;
        num %= 10;
        strcat(result, tens[ten_part]);
        if (num > 0) {
            strcat(result, " ");
        }
    }

    if (num >= 10 && num < 20) {
        strcat(result, teens[num - 10]);
        return;
    }

    if (num > 0 && num < 10) {
        strcat(result, ones[num]);
    }
}

int main() {
    int numrecieved; // Variable to store the number received from the client
    int serversocket, port; // Server socket descriptor and port number
    struct sockaddr_in serveraddr, clientaddr; // Server and client address structures
    socklen_t len; // Variable to store the length of client address
    char message[100]; // Char array to store the response message

    serversocket = socket(AF_INET, SOCK_DGRAM, 0); // Create a UDP socket

    bzero((char*)&serveraddr, sizeof(serveraddr)); // Initialize server address to zero
    serveraddr.sin_family = AF_INET; // Set address family to AF_INET

    // Get port number from the user
    printf("Enter the port number: ");
    scanf("%d", &port);
    serveraddr.sin_port = htons(port); // Convert port to network byte order
    serveraddr.sin_addr.s_addr = INADDR_ANY; // Bind to all interfaces

    // Bind the socket to the server address
    bind(serversocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    printf("\nWaiting for the client connection...\n");

    len = sizeof(clientaddr); // Store the length of the client address
    bzero((char*)&clientaddr, sizeof(clientaddr)); // Initialize client address to zero

    // Receive the number from the client
    recvfrom(serversocket, &numrecieved, sizeof(numrecieved), 0, (struct sockaddr*)&clientaddr, &len);
    printf("\nReceived number from client: %d\n", numrecieved);

    // Convert the number to words
    char result[100] = ""; // String to store the result
    number_to_words(numrecieved, result);

    // Send the result back to the client
    sendto(serversocket, result, sizeof(result), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));

    close(serversocket); // Close the socket
}
