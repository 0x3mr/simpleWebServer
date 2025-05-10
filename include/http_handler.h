#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

/**
 * Thread function to handle a single HTTP request
 * 
 * @param arg Pointer to client socket file descriptor
 * @return NULL
 */
void *handle_request(void *arg);

/**
 * Send a 404 Not Found response to the client
 * 
 * @param client_fd Client socket file descriptor
 */
void send_404(int client_fd);

/**
 * Sanitize a request path to prevent directory traversal
 * 
 * @param req_path The requested path
 * @return Sanitized path (caller must free) or NULL if path is invalid
 */
char *sanitize_path(const char *req_path);

#endif