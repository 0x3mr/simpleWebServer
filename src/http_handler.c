#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../include/config.h"
#include "../include/http_handler.h"
#include "../include/logger.h"
#include "../include/utils.h"

/**
 * Thread function to handle a single HTTP request
 * 
 * @param arg Pointer to client socket file descriptor
 * @return NULL
 */
void *handle_request(void *arg)
{
    int client_fd = *(int *)arg;
    free(arg);
    
    char buf[BUF_SIZE];
    
    int bytes = read(client_fd, buf, BUF_SIZE - 1);
    if (bytes <= 0)
    {
        close(client_fd);
        return NULL;
    }
    buf[bytes] = '\0';

    char method[MAX_METHOD_LENGTH], path[MAX_URI_LENGTH];
    if (sscanf(buf, "%7s %1023s", method, path) != 2)
    {
        send_404(client_fd);
        close(client_fd);
        return NULL;
    }

    if (strcmp(method, "GET") != 0)
    {
        send_404(client_fd);
        close(client_fd);
        return NULL;
    }

    char *safe_path = sanitize_path(path);
    if (!safe_path)
    {
        send_404(client_fd);
        close(client_fd);
        return NULL;
    }

    char full_path[MAX_PATH_LENGTH];
    snprintf(full_path, sizeof(full_path), "%s/%s", base_dir, safe_path);
    
    const char *mime_type = get_mime_type(safe_path);
    
    free(safe_path);

    int fd = open(full_path, O_RDONLY);
    if (fd < 0)
    {
        send_404(client_fd);
        close(client_fd);
        return NULL;
    }

    struct stat st;
    if (fstat(fd, &st) < 0)
    {
        perror("Failed to get file stats");
        close(fd);
        send_404(client_fd);
        close(client_fd);
        return NULL;
    }

    void *file_data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_data == MAP_FAILED)
    {
        perror("Failed to memory map file");
        close(fd);
        send_404(client_fd);
        close(client_fd);
        return NULL;
    }

    dprintf(client_fd, "HTTP/1.0 200 OK\r\n");
    dprintf(client_fd, "Content-Type: %s\r\n", mime_type);
    dprintf(client_fd, "Content-Length: %ld\r\n\r\n", st.st_size);
    write(client_fd, file_data, st.st_size);

    munmap(file_data, st.st_size);
    close(fd);

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    getpeername(client_fd, (struct sockaddr *)&addr, &len);
    log_request(inet_ntoa(addr.sin_addr), path);

    close(client_fd);
    return NULL;
}

/**
 * Send a 404 Not Found response to the client
 * 
 * @param client_fd Client socket file descriptor
 */
void send_404(int client_fd)
{
    const char *msg = "HTTP/1.0 404 Not Found\r\n"
                      "Content-Type: text/html\r\n"
                      "Content-Length: 162\r\n\r\n"
                      "<html><head><title>404 Not Found</title></head>"
                      "<body><h1>404 Not Found</h1>"
                      "<p>The requested resource could not be found on this server.</p>"
                      "</body></html>";
    write(client_fd, msg, strlen(msg));
}

/**
 * Sanitize a request path to prevent directory traversal
 * 
 * @param req_path The requested path
 * @return Sanitized path (caller must free) or NULL if path is invalid
 */
char *sanitize_path(const char *req_path)
{
    if (strstr(req_path, ".."))
    {
        return NULL;
    }
    
    if (req_path[0] == '/')
    {
        req_path++;
    }
    
    if (req_path[0] == '\0')
    {
        return strdup("index.html");
    }
    
    return strdup(req_path);
}