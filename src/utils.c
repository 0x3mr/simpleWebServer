#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../include/utils.h"

/**
 * Create directory if it doesn't exist
 * 
 * @param path Directory path
 * @return 0 on success, -1 on failure
 */
int ensure_directory_exists(const char *path)
{
    struct stat st = {0};
    
    if (stat(path, &st) == -1)
    {
        if (mkdir(path, 0755) == -1)
        {
            perror("Failed to create directory");
            return -1;
        }
    }
    else if (!S_ISDIR(st.st_mode))
    {
        fprintf(stderr, "%s exists but is not a directory\n", path);
        return -1;
    }
    
    return 0;
}

/**
 * Get file MIME type based on extension
 * 
 * @param filename Filename to check
 * @return MIME type string
 */
const char *get_mime_type(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    
    if (dot)
    {
        if (strcasecmp(dot, ".html") == 0 || strcasecmp(dot, ".htm") == 0)
        {
            return "text/html";
        }
        else if (strcasecmp(dot, ".txt") == 0)
        {
            return "text/plain";
        }
        else if (strcasecmp(dot, ".jpg") == 0 || strcasecmp(dot, ".jpeg") == 0)
        {
            return "image/jpeg";
        }
        else if (strcasecmp(dot, ".png") == 0)
        {
            return "image/png";
        }
        else if (strcasecmp(dot, ".gif") == 0)
        {
            return "image/gif";
        }
        else if (strcasecmp(dot, ".css") == 0)
        {
            return "text/css";
        }
        else if (strcasecmp(dot, ".js") == 0)
        {
            return "application/javascript";
        }
        else if (strcasecmp(dot, ".pdf") == 0)
        {
            return "application/pdf";
        }
        else if (strcasecmp(dot, ".xml") == 0)
        {
            return "application/xml";
        }
        else if (strcasecmp(dot, ".json") == 0)
        {
            return "application/json";
        }
    }
    
    return "application/octet-stream";
}