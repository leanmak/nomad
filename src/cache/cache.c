#include <stdio.h>
#include <winsock2.h>
#include <string.h>

#include "cache.h"
#include "../utils.h"

FileCache *NewFileCache() {
    FileCache *cache = malloc(sizeof(FileCache));
    if(!cache) {
        printf("Failed to allocate memory for a new file cache struct.\n");
        return NULL;
    }

    ZeroMemory(cache, sizeof(FileCache));

    char *file_paths[2] = {"index.html", "404.html"};
    cache->length = 2;

    CachedFile *prev = NULL;
    for(int i = 0; i < 2; i++) {
        char *path = file_paths[i];

        char *file_content = GetFileContent(path);
        if(!file_content) {
            printf("Failed to fetch file content at %s.\n", path);
            return NULL;
        }

        CachedFile *cached_file = malloc(sizeof(CachedFile));
        if(!cached_file) {
            printf("Failed to allocate memory for a new cached file struct.");
            return NULL;
        }

        ZeroMemory(cached_file, sizeof(CachedFile));

        strncpy(cached_file->name, path, sizeof(cached_file->name)-1);
        cached_file->name[sizeof(cached_file->name)-1] = '\0';
        cached_file->content = file_content;
        cached_file->length = strlen(file_content);

        if(!cache->head) {
            cache->head = cached_file;
        } else {
            prev->next = cached_file;
        }

        prev = cached_file;
    }

    return cache;
}

CachedFile *GetCachedFile(const char *file_name, FileCache *cache) {
    CachedFile *curr = cache->head;

    while(curr != NULL) {
        if(strcmp(curr->name, file_name) == 0) {
            return curr;
        }
        
        curr = curr->next;
    }

    return NULL;
}