#include "host.h"

struct host_t* Server(int port, int buffer_size){
    struct host_t* server = malloc(sizeof(struct host_t));
    server->port = port;
    server->buffer_size = buffer_size;
    server->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->sockfd == -1) {
        fprintf(stderr, "[nnhost] Failed to create socket: %d, exiting\n", errno);
        return NULL;
    } else {
        fprintf(stderr, "[nnhost] Socket Created..\n");
    }

    server->address.sin_family = AF_INET;
    server->address.sin_port = htons(server->port);
    server->address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server->sockfd, (const struct sockaddr*)&server->address, sizeof(server->address)) < 0) goto bind_fail;
    fprintf(stderr, "[nnhost] Bound to address structure..\n");

    server->input_buffer = malloc(sizeof(char) * buffer_size);
    server->output_buffer = malloc(sizeof(char) * buffer_size);
    fprintf(stderr, "[nnhost] IO buffers of size %d bytes initialized..\n", buffer_size);

    return server; 
    
    bind_fail:
    fprintf(stderr, "[nnhost] Failed to bind to address structure: %d\n", errno);
    close(server->sockfd);
    fprintf(stderr, "[nnhost] Socket closed.\n");
    return NULL;
}

struct host_t* Client(int port, int buffer_size){
    struct host_t* client = malloc(sizeof(struct host_t));
    client->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (client->sockfd == -1)
    {
        fprintf(stderr, "[nnhost] Failed to initialize socket: %d\n", errno);
        return NULL;
    } else {
        fprintf(stderr, "[nnhost] Socket created..\n");
    }

    client->port = port;
    client->buffer_size = buffer_size;
    client->input_buffer = malloc(sizeof(char) * buffer_size);
    client->output_buffer = malloc(sizeof(char) * buffer_size);
    fprintf(stderr, "[nnhost] IO buffers of size %d bytes initialized..\n", buffer_size);

    return client;
}

void closeHost(struct host_t* server){
    close(server->sockfd);
    fprintf(stderr, "[nnhost] Host socket closed.\n");
    free(server->input_buffer);
    free(server->output_buffer);
    fprintf(stderr, "[nnhost] IO buffer structures freed.\n");
    free(server);
    fprintf(stderr, "[nnhost] Host structure freed. \n");
}

int Listen(struct host_t* server, struct ProgramState* state){
    
    if (listen(server->sockfd, 5) == -1) goto listen_fail;
    fprintf(stderr, "[nnhost] TCP listening on port %d..\n", server->port);

    int cli_sockfd = 0, bytes_recv = 0, bytes_sent = 0;

    while (1) {
        cli_sockfd = accept(server->sockfd, NULL, NULL);
        if (cli_sockfd == -1) {
            fprintf(stderr, "[nnhost] Failed to accept a connection\n");
            continue;
        }
        
        fprintf(stderr, "[nnhost] Accepted connection\n");
        memset(server->input_buffer, 0, server->buffer_size * sizeof(char));
        char current_message[server->buffer_size];
        while(1) {
            memset(server->input_buffer, 0, server->buffer_size * sizeof(char));
            bytes_recv = read(cli_sockfd, server->input_buffer, server->buffer_size - 1);
            if (bytes_recv <= 0) 
            {
                fprintf(stderr, "[nnhost] Error reading from socket: %d\n", errno);
                goto close;
            }

            snprintf(current_message, server->buffer_size * sizeof(char), "%s%s", current_message, server->input_buffer);
            if (server->input_buffer[bytes_recv - 1] == '\0') {
                fprintf(stderr, "[nnhost] Received message of size %d : %s \n", strlen(current_message), current_message);
                

                
                if (strcmp(server->input_buffer, "exit") == 0)
                {
                    fprintf(stderr, "[nnhost] Received exit signal\n");
                    memset(current_message, 0, server->buffer_size * sizeof(char));
                    close(cli_sockfd);
                    goto close_success;
                    break;
                }
                
                memset(server->output_buffer, 0, sizeof(char) * server->buffer_size);
                cmdres(state, (const char*)current_message, server->output_buffer, sizeof(char) * server->buffer_size);
                // sprintf(server->output_buffer, "Got your message of size %d\0", strlen(current_message));



                bytes_sent = write(cli_sockfd, server->output_buffer, strlen(server->output_buffer) + 1);
                if (bytes_sent <= 0) 
                {
                    fprintf(stderr, "[nnhost] Error writing to buffer.");
                    goto close;
                }
                fprintf(stderr, "[nnhost] Sent response of size %d\n", bytes_sent);

                memset(current_message, 0, strlen(current_message));
            }
        }
    }

    close_success:
    closeHost(server);
    return 0;

    listen_fail:
    fprintf(stderr, "[nnhost] Failed to listen: %d, exiting\n", errno);

    close:
    closeHost(server);
    return -1;
}

int Connect(struct host_t* client, const char* hostname){
    int ret, bytes_sent, bytes_recv, chars_read;
    int getline_n = client->buffer_size * sizeof(char);
    struct hostent* server = gethostbyname(hostname);
    client->address.sin_family = AF_INET;
    client->address.sin_port = htons(client->port);
    memcpy(&client->address.sin_addr.s_addr, server->h_addr, server->h_length);

    if (server == NULL) {
        fprintf(stderr, "[nnhost] No such host.\n");
        closeHost(client);
        return -1;
    }

    ret = connect(client->sockfd, (const struct sockaddr*)&client->address, sizeof(struct sockaddr_in));
    if (ret == -1) {
        fprintf(stderr, "[nnhost] Failed to connect to host %s: %d\n", hostname, errno);
        closeHost(client);
        return -1;
    }

    char current_message[client->buffer_size];
    const char* prompt = "> ";
    fprintf(stderr, "[nnhost] Connection established with %s.\n", hostname);
    while(strcmp(client->output_buffer, "exit") != 0) {
        printf("%s", prompt);
        memset(client->output_buffer, 0, client->buffer_size * sizeof(char));
        chars_read = getline(&client->output_buffer, (size_t*)&getline_n, stdin);
        client->output_buffer[chars_read - 1] = '\0';
        fprintf(stderr, "[nnhost] Sending %d bytes\n", chars_read);
        bytes_sent = write(client->sockfd, client->output_buffer, chars_read);
        if (bytes_sent < 0) goto write_fail;

        memset(client->input_buffer, 0, client->buffer_size * sizeof(char));
        
        bytes_recv = read(client->sockfd, client->input_buffer, client->buffer_size - 1);
        if (bytes_recv < 0) goto read_fail;

        if (client->input_buffer[bytes_recv - 1] == '\0') {
            fprintf(stderr, "%s", client->input_buffer);
        }

        if (strcmp(client->output_buffer, "exit") == 0) goto close;
    }

    closeHost(client);
    return 0;
    
    read_fail:
    fprintf(stderr, "[nnhost] Failed reading from socket: %d\n", errno);
    goto close;

    write_fail:
    fprintf(stderr, "[nnhost] Error writing to socket\n");
    goto close;

    close:
    closeHost(client);
    return -1;
}