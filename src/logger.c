#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "../include/config.h"
#include "../include/logger.h"

FILE *log_fp = NULL;
pthread_mutex_t log_lock = PTHREAD_MUTEX_INITIALIZER;

/**
 * Initialize the logger
 * 
 * @return 0 on success, -1 on failure
 */
int logger_init(void)
{
    log_fp = fopen(LOG_FILE, "a");
    if (!log_fp)
    {
        perror("Failed to open log file");
        return -1;
    }
    
    time_t now = time(NULL);
    char *ts = ctime(&now);
    ts[strlen(ts)-1] = '\0';
    
    fprintf(log_fp, "[%s] Server started\n", ts);
    fflush(log_fp);
    
    return 0;
}

/**
 * Clean up logging resources
 */
void logger_cleanup(void)
{
    if (log_fp)
    {
        time_t now = time(NULL);
        char *ts = ctime(&now);
        ts[strlen(ts)-1] = '\0';
        
        fprintf(log_fp, "[%s] Server shutdown\n", ts);
        fclose(log_fp);
        log_fp = NULL;
    }
}

/**
 * Log an HTTP request with timestamp and client info
 * 
 * @param ip Client IP address
 * @param path Requested path
 */
void log_request(const char *ip, const char *path)
{
    time_t now = time(NULL);
    char *ts = ctime(&now);
    ts[strlen(ts)-1] = '\0';
    
    pthread_mutex_lock(&log_lock);
    
    fprintf(log_fp, "[%s] %s requested %s\n", ts, ip, path);
    fflush(log_fp);
    
    pthread_mutex_unlock(&log_lock);
}