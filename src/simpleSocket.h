#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<ctype.h>

bool portVarify(const char* port);

void soket_recv(int sockfd, char* buffer, int BUFFER_SIZE);

void socket_send_file_size(int sockfd, FILE* fp, int buffer_size);

void setTimeout(int sockfd, int send_time, int recv_time);