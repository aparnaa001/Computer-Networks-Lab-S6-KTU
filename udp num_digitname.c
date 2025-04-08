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

// Function to convert each digit to its word equivalent
void digits_to_words(int num, char *result) {
    // Array of words for digits
    const char *digits[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    // If the number is zero, directly return "zero"
    if (num == 0) {
        strcpy(result, "zero");
        return;
    }

    // Process each digit and convert it to words
    char temp[100] = ""; // Temporary string to hold the word result
    int first_digit = 1; // Flag to handle formatting (comma between words)

    while (num > 0) {
        int digit = num % 10; // Get the last digit
        num /= 10; // Remove the last digit

        // Add the word for the digit to the result string
        if (first_digit) {
            sprintf(temp, "%s", digits[digit]);
            first_digit = 0;
        } else {
            sprintf(temp, "%s, %s", digits[digit], temp);
        }
    }

    strcpy(result, temp); // Copy the result to the output string
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

    // Convert the number's digits to words
    char result[100] = ""; // String to store the result
    digits_to_words(numrecieved, result);

    // Send the result back to the client
    sendto(serversocket, result, sizeof(result), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));

    close(serversocket); // Close the socket
}
