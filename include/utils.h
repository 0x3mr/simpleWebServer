#ifndef UTILS_H
#define UTILS_H

/**
 * Create directory if it doesn't exist
 * 
 * @param path Directory path
 * @return 0 on success, -1 on failure
 */
int ensure_directory_exists(const char *path);

/**
 * Get file MIME type based on extension
 * 
 * @param filename Filename to check
 * @return MIME type string
 */
const char *get_mime_type(const char *filename);

#endif