#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <string> 
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>
#include "simpleSocket.h"

// #define BUFFER_SIZE 1024 
// #define SEND_SIZE 1024 

int main(int argc, char const *argv[]) 
{ 
    int sockfd = 0;
    struct sockaddr_in serv_addr; 
    char buffer[BUFFER_SIZE] = {0}; 
    char* sendline;
    if(argc != 3){
        printf("usage: ./rsclient <ipaddress> <port>\n");
        return 0;
    }

    // Varify port
    if(!portVarify(argv[2])){
        printf("\n Port invalid\n"); 
        return 0;
    }

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("Socket creation error %s(errno: %d)\n", strerror(errno),errno);
        return -1; 
    }else{
        printf("Socket created \n"); 
    }
    // Create socket address
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    int port_num = atoi(argv[2]);
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(port_num); 

    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)  
    { 
        printf("\n Invalid address: %s\n",argv[1]);
        return -1; 
    } 
    // Socket connection
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\n Connection Failed %s(errno: %d)\n",strerror(errno),errno);
        return -1; 
    } 
    // Set timeout
    setTimeout(sockfd, 3, 3);

    // // Send hello message 
    // printf("Send hello to server:\n%s\n", helloline);
    // if( send(sockfd, helloline, strlen(helloline), 0) < 0){
    //     printf("Send hello error: %s(errno: %d)\n", strerror(errno), errno);
    //     return 0;
    // }

    // printf("*Client hello sent\n"); 
    // Recive data

    while(1){
        // Send command
        // Read line
        sendline = readline("client $ ");
        //sendline = "exit";
        if(strlen(sendline) > SEND_SIZE){
            printf("Send command error: Command too long");
            continue;
        }
        
        printf("*Send command to server:\n%s\n", sendline);
        simpleSocketSend(sockfd, sendline, strlen(sendline));
        printf("*Client command sent\n"); 

        //if exit command
        if(!strcmp(sendline, "exit")){
            //exit 
            //free sendLine
            free(sendline);
            printf("Exit the server\n");
            break;
        }
        //free sendLine
        free(sendline);
        //Wait and Recive data
        printf("*Server return message: \n");
        while(1){
            memset(buffer, 0, BUFFER_SIZE); // Clean buffer before each recv

            if(simpleSocketRecv(sockfd, buffer, BUFFER_SIZE) > 0){
                printf("%s", buffer);
                continue;
            }else{
                printf("%s", buffer);
                printf("*End of data \n");
                break;
            }
        }
    }
    // End of connection
    close(sockfd);
    return 0; 
} 