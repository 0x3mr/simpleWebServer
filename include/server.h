#ifndef SERVER_H
#define SERVER_H

/**
 * Initialize the server
 * 
 * @param port Port number to listen on
 * @return Server socket file descriptor or -1 on error
 */
int server_init(int port);

/**
 * Run the server's main loop
 * 
 * @param server_fd Server socket file descriptor
 */
void server_run(int server_fd);

/**
 * Clean up server resources
 * 
 * @param server_fd Server socket file descriptor
 */
void server_cleanup(int server_fd);

#endif