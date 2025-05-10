#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../include/config.h"
#include "../include/server.h"
#include "../include/http_handler.h"

/**
 * Initialize the server
 * 
 * @param port Port number to listen on
 * @return Server socket file descriptor or -1 on error
 */
int server_init(int port)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Failed to create socket");
        return -1;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("Failed to set socket options");
        close(server_fd);
        return -1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = INADDR_ANY
    };

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Failed to bind socket");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, MAX_PENDING_CONNECTIONS) < 0)
    {
        perror("Failed to listen on socket");
        close(server_fd);
        return -1;
    }

    return server_fd;
}

/**
 * Run the server's main loop
 * 
 * @param server_fd Server socket file descriptor
 */
void server_run(int server_fd) {
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int *client_fd = malloc(sizeof(int));
        if (!client_fd)
        {
            perror("Failed to allocate memory");
            continue;
        }
        
        *client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (*client_fd < 0)
        {
            free(client_fd);
            continue;
        }

        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_request, (void *)client_fd) != 0)
        {
            perror("Failed to create thread");
            close(*client_fd);
            free(client_fd);
            continue;
        }
        
        pthread_detach(thread);
    }
}

/**
 * Clean up server resources
 * 
 * @param server_fd Server socket file descriptor
 */
void server_cleanup(int server_fd)
{
    close(server_fd);
}