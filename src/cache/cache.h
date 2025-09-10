#pragma once

#ifndef CACHE_H
#define CACHE_H

// Holds information about a cached file
typedef struct cached_file_s {
    char name[16];
    char *content;
    int length;
    struct cached_file_s *next;
} CachedFile;

// A linked-list type structure connecting all cached files
typedef struct file_cache_s {
    CachedFile *head;
    int length;
} FileCache;

/**
 * @brief Creates a new file cache. Should only be used once in a program's lifetime.
 * 
 * @return A pointer to a `FileCache` struct on success.
 * @return `NULL` on failure.
 */
FileCache *NewFileCache();

/**
 * @brief Fetches a cached file.
 * 
 * @return A pointer to the cached file on success.
 * @return `NULL` on failure.
 */
CachedFile *GetCachedFile(const char *file_name, FileCache *cache);

#endif