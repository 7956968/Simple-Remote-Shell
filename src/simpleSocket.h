#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<ctype.h>

#define BUFFER_SIZE 1024 
#define SEND_SIZE 1024 


bool portVarify(const char* port);

void soket_recv(int sockfd, char* buffer, int buffer_size);

void socket_send_file_size(int sockfd, FILE* fp, int buffer_size);

void setTimeout(int sockfd, int send_time, int recv_time);

void simpleSocketSend(int sockfd, char* buffer, int buffer_size);
int simpleSocketRecv(int sockfd, char* buffer, int buffer_size);