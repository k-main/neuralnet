#include "model.h"
#include "io.h"

#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

#define PORT 3200
#define BUFFER_SIZE 128

int main(int argc, char* argv[]) {
    char in_buffer[BUFFER_SIZE];
    char out_buffer[BUFFER_SIZE];

    int ret, bytes_recv, bytes_sent;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0), cli_sockfd;
    if (sockfd == -1) goto socket_creation_err;
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(sockfd, (const struct sockaddr*)&address, sizeof(address)) < 0) goto socket_bind_err; 
    if (listen(sockfd, 5) < 0) goto socket_listen_err;

    printf("Server listening on port %d\n", PORT);

    struct sockaddr_in cli_address;
    int cli_addr_sz = sizeof(cli_address);
    cli_address.sin_family = AF_INET;
    
    cli_sockfd = accept(sockfd, (struct sockaddr*)&cli_address, &cli_addr_sz);
    if (cli_sockfd == -1) goto close_sock;
    printf("Accepted client\n");
    
    while(strcmp("exit\n", in_buffer) != 0) {
        memset(in_buffer, 0, sizeof(in_buffer));
        memset(out_buffer, 0, sizeof(out_buffer));
        bytes_recv = read(cli_sockfd, in_buffer, sizeof(in_buffer));
        if (bytes_recv <= 0) 
        {
            printf("Error reading from socket: %d\n", errno);
            close(cli_sockfd);
        }
    
        printf("(%d) client: %s\n", bytes_recv, in_buffer);
        sprintf(out_buffer, "got your message of size %d\n", bytes_recv);
    
        bytes_sent = write(cli_sockfd, out_buffer, strlen(out_buffer));
        if (bytes_sent <= 0) 
        {
            printf("Error writing to buffer: %d\n", errno);
            goto close_sock;
        } 
        printf("(%d) server: %s\n", bytes_sent, out_buffer);
        
    }



    close(cli_sockfd);
    close(sockfd);
    return 0;

    srand(getpid());
    struct model Model = loadModel("model1");
    dumpInfo(&Model);
    tearDown(&Model);
    return 0;


    socket_creation_err:
    printf("Error creating socket: %d\n", errno);
    exit(EXIT_FAILURE);

    socket_listen_err:
    printf("Error listening on socket: %d\n", errno);
    close(sockfd);
    exit(EXIT_FAILURE);

    socket_bind_err:
    printf("Error binding socket: %d\n", errno);
    close(sockfd);
    exit(EXIT_FAILURE);

    close_sock:
    if (cli_sockfd != -1) close(cli_sockfd);
    close(sockfd);
    exit(EXIT_FAILURE);
}