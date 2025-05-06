#include "nnhost.h"

struct server_t* Server(int port, int buffer_size){
    struct server_t* server = malloc(sizeof(struct server_t));
    server->port = port;
    server->buffer_size = buffer_size;
    server->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->sockfd == -1) {
        printf("[server] Failed to create socket: %d, exiting\n", errno);
    } else {
        printf("[server] Socket Created..\n");
    }

    server->address.sin_family = AF_INET;
    server->address.sin_port = htons(server->port);
    server->address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server->sockfd, (const struct sockaddr*)&server->address, sizeof(server->address)) < 0) goto bind_fail;
    printf("[server] Bound to address structure..\n");

    server->input_buffer = malloc(sizeof(char) * buffer_size);
    server->output_buffer = malloc(sizeof(char) * buffer_size);
    printf("[server] IO buffers of size %d bytes initialized..\n", buffer_size);

    return server; 
    
    bind_fail:
    printf("[server] Failed to bind to address structure: %d\n", errno);
    closeServer(server);
}

void closeServer(struct server_t* server){
    close(server->sockfd);
    printf("[server] Server socket closed.\n");
    free(server->input_buffer);
    free(server->output_buffer);
    printf("[server] IO buffer structures freed.\n");
    free(server);
    printf("[server] Server structure freed. \n");
}

int Listen(struct server_t* server){
    
    if (listen(server->sockfd, 5) == -1) goto listen_fail;
    printf("[server] Listening on port %d..\n", server->port);

    int cli_sockfd = 0, bytes_recv = 0, bytes_sent = 0;

    while (cli_sockfd != -1) {
        cli_sockfd = accept(server->sockfd, NULL, NULL);
        if (cli_sockfd == -1) {
            printf("[server] Failed to accept a connection\n");
            continue;
        }
        
        printf("[server] Accepted connection");
        while(1) {
            memset(server->input_buffer, 0, sizeof(server->input_buffer));
            memset(server->output_buffer, 0, sizeof(server->output_buffer));
            bytes_recv = read(cli_sockfd, server->input_buffer, sizeof(server->input_buffer));
            if (bytes_recv <= 0) 
            {
                printf("[server] Error reading from socket\n");
                goto close;
            }

            printf("[server] Received message of size %d : %s \n", bytes_recv, server->input_buffer);

            if (strcmp(server->input_buffer, "exit\n") == 0)
            {
                printf("[server] Received exit signal\n");
                close(cli_sockfd);
                cli_sockfd = -1;
                break;
            }

            sprintf(server->output_buffer, "[server] Providing generic response to message of size %d\n", bytes_recv);
            bytes_sent = write(cli_sockfd, server->output_buffer, strlen(server->output_buffer));
            if (bytes_sent <= 0) 
            {
                printf("[server] Error writing to buffer.");
                goto close;
            }
            printf("[server] Sent response of size %d\n", bytes_sent);
        }
    }

    closeServer(server);
    return 0;
    listen_fail:
    printf("[server] Failed to listen: %d, exiting\n", errno);

    close:
    closeServer(server);
    return -1;
}