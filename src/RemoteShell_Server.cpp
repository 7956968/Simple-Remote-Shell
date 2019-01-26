#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<ctype.h>
#include "simpleSocket.h"

#define BUFFER_SIZE 1024
#define FILE_LEN_BUFFER 100


int main(int argc, char** argv){
    int  listenfd, connfd;
    struct sockaddr_in  servaddr;
    char  buffer[1024];
    int  n;

    if(argc != 2){
        printf("usage: ./rsserver <port>\n");
        return 0;
    }

    // Varify port
    if(!portVarify(argv[1])){
        printf("\n Port invalid\n"); 
    }
    // Listen socket create
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }
    int port_num = atoi(argv[1]);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port_num);

    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }
    //listenning
    if( listen(listenfd, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    printf("======waiting for client's request======\n");
    while(1){
        if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            continue;
        }else{
            // Set timeout
            setTimeout(connfd, 3, 0);
            //connected
            while(1){
                //connection success receiving cmd
                memset(buffer, 0, BUFFER_SIZE);
                int recv_size = (int)recv(connfd, buffer, BUFFER_SIZE, 0);
                if( recv_size >0 )
                {
                    // Handle the buffer
                    printf("receive message from client: %s\n", buffer);
                }
                else{
                    // Handle socket recv error
                    if((recv_size<0) &&(recv_size == EAGAIN||recv_size == EWOULDBLOCK||recv_size == EINTR)) //error code, connection doesn't fail continue
                    {
                        printf("\n Socket error %s(errno: %d)\n", strerror(errno),errno);
                        continue;
                    }
                    printf("*End of receive \n");
                    break;
                }

                //exit the server
                if(!strcmp(buffer, "exit")){
                    //close connection
                    printf("*Close connection \n");
                    close(connfd);
                    break;
                }
                //printf("*Run command: %s\n", buffer);

                // Handle the command
                FILE *fp;
                const char* command_tmp = " 2>&1"; // get stderr
                char *command = (char*)malloc(strlen(command_tmp)+strlen(buffer)+1);
                //construct command with error return
                strcpy(command, buffer);
                strcat(command, command_tmp);
                
                printf("*Run command: %s\n", command);
                //char *command = buffer;
                fp = popen(command, "r");
                free(command);
                if(fp == NULL){
                    printf("popen error!\n");
                    exit(1);
                }

                //socket_send_file_size(connfd, fp, FILE_LEN_BUFFER);
 
                int block_len = 0;
                while( (block_len = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
                {
                    // Send data
                    // if (send(connfd, buffer, block_len, 0) < 0)
                    // {
                    //     printf("Send data error: %s(errno: %d)\n", strerror(errno), errno);
                    //     break;
                    // }
                    // printf("*Sending: %s\n", buffer);
                    simpleSocketSend(connfd, buffer, block_len);
                    bzero(buffer, sizeof(buffer));
                }
                // Close file 
                if(pclose(fp) == -1){
                    printf("Close file error!\n");
                    exit(1);
                }
            }
        }
    }
    close(listenfd);
    return 0;
}