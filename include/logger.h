#ifndef LOGGER_H
#define LOGGER_H

#include <pthread.h>

extern FILE *log_fp;
extern pthread_mutex_t log_lock;

/**
 * Initialize the logger
 * 
 * @return 0 on success, -1 on failure
 */
int logger_init(void);

/**
 * Clean up logging resources
 */
void logger_cleanup(void);

/**
 * Log an HTTP request with timestamp and client info
 * 
 * @param ip Client IP address
 * @param path Requested path
 */
void log_request(const char *ip, const char *path);

#endif