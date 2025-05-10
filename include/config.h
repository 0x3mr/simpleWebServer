#ifndef CONFIG_H
#define CONFIG_H

#define BUF_SIZE 4096

#define LOG_FILE "logs/access.log"

#define MAX_PENDING_CONNECTIONS 10

#define MAX_PATH_LENGTH 2048
#define MAX_METHOD_LENGTH 8
#define MAX_URI_LENGTH 1024

extern char *base_dir;

#endif