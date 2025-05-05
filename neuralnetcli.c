#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memcpy

#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

#define PORT 3200
#define BUFFER_SIZE 128

int main(int argc, char* argv[]){
    char in_buffer[BUFFER_SIZE];
    char out_buffer[BUFFER_SIZE];

    int bytes_sent, bytes_recv;

    const char* host = "127.0.0.1";
    int ret;
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
        printf("Error creating socket: %d", errno);
        exit(EXIT_FAILURE);
    }

    struct hostent* server = gethostbyname(host);
    if (server == NULL) 
    {
        printf("No such host\n");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in svr_addr;
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htons(PORT);
    memcpy(&svr_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    ret = connect(sockfd, (const struct sockaddr*)&svr_addr, sizeof(svr_addr));
    if (ret < 0) 
    {
        printf("Error connecting: %d\n", errno);
        exit(EXIT_FAILURE);
    } else {
        printf("Connection established\n Message: ");
    }

    fgets(out_buffer, sizeof(out_buffer) - 1, stdin);
    while (strcmp(out_buffer, "exit") != 0) {
        bytes_sent = write(sockfd, out_buffer, strlen(out_buffer));
        if (bytes_sent < 0)
        {
            printf("Error writing to socket %d\n", errno);
            exit(EXIT_FAILURE);
        }

        memset(in_buffer, 0, sizeof(in_buffer));
        bytes_recv = read(sockfd, in_buffer, sizeof(in_buffer) - 1);
        if (bytes_recv < 0)
        {
            printf("Failed reading from socket %d\n", errno);
            exit(EXIT_FAILURE);
        }
        printf("(%d) server: %s", bytes_recv, in_buffer);
        memset(out_buffer, 0, sizeof(out_buffer));
        fgets(out_buffer, sizeof(out_buffer) - 1, stdin);
    }

    bytes_recv = read(sockfd, in_buffer, sizeof(in_buffer) - 1);
    if (bytes_recv < 0)
    {
        printf("Failed reading from socket %d\n", errno);
        exit(EXIT_FAILURE);
    }
    printf("Server Response: \n %s", in_buffer);

    close(sockfd);
    return 0;
}