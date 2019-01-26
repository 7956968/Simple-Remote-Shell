#include"simpleSocket.h"

bool portVarify(const char* port){
    for(int i = 0; i < strlen(port); ++i){
        if(!isdigit(port[i])){
            //inclued non-digit
            return false;
        }
    }
    if(atoi(port) < 0 || atoi(port) > 65535){
        //invalid number
        return false;
    }
    return true;
}

void socket_send_file_size(int sockfd, FILE* fp, int buffer_size){
    if(fp == NULL){
        printf("File open error!");
    }
    int fileLen = 0;
    int seek_v = 0;
    // Get file size
    seek_v = fseek(fp, 0, SEEK_END); //to the end of file
    fileLen = ftell(fp); //*return long use int might lost accuracy
    fseek(fp, 0, SEEK_SET); //return to start
    char  len_buffer[buffer_size];
    memset(len_buffer,0,sizeof(len_buffer));
    sprintf(len_buffer,"%d",fileLen);
    len_buffer[buffer_size - 1] = '\0';
    printf("*Send file length: %s\n", len_buffer);
    printf("*Seek: %i\n", seek_v);
    if (send(sockfd, len_buffer, buffer_size, 0) < 0)
    {
        printf("Send data error: %s(errno: %d)\n", strerror(errno), errno);
    }
}

void simpleSocketSend(int sockfd, char* buffer, int buffer_size){
    // Send data
    if (send(sockfd, buffer, buffer_size, 0) < 0)
    {
        printf("Send data error: %s(errno: %d)\n", strerror(errno), errno);
    }
    printf("*Sending: \n%s\n", buffer);
}

int simpleSocketRecv(int sockfd, char* buffer, int buffer_size){
        int recv_size = (int)recv(sockfd, buffer, buffer_size, 0);
        if( recv_size >0 )
        {
            // Handle the buffer
            if(recv_size < buffer_size){
                return 0;
            }else{
                return 1;
            }
            //printf("*Received message: \n%s\n",buffer );
        }
        else{
            // Handle socket recv error
            if((recv_size<0) &&(recv_size == EAGAIN||recv_size == EWOULDBLOCK||recv_size == EINTR)) //error code, connection doesn't fail continue
            {
                printf("\n Socket error %s(errno: %d)\n", strerror(errno),errno);
                return -1;
            }
            //printf("*End of receive \n");
            return 0;
        }
}

void setTimeout(int sockfd, int send_time, int recv_time){
    struct timeval timeout_send={send_time,0};//3s
    struct timeval timeout_recv={recv_time,0};//3s

    int ret_send=setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout_send,sizeof(timeout_send));
    int ret_recv=setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout_recv,sizeof(timeout_recv));

}