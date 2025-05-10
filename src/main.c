#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/config.h"
#include "../include/server.h"
#include "../include/logger.h"
#include "../include/utils.h"

char *base_dir;

/**
 * Program entry point
 */
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <port> <directory>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    base_dir = argv[2];

    if (ensure_directory_exists("logs") != 0)
    {
        fprintf(stderr, "Failed to create logs directory\n");
        return EXIT_FAILURE;
    }

    if (logger_init() != 0)
    {
        return EXIT_FAILURE;
    }

    int server_fd = server_init(port);
    if (server_fd < 0)
    {
        logger_cleanup();
        return EXIT_FAILURE;
    }

    printf("Server listening on port %d, serving files from '%s'\n", port, base_dir);

    server_run(server_fd);

    server_cleanup(server_fd);
    logger_cleanup();

    return EXIT_SUCCESS;
}