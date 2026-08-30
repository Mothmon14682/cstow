#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "fs.h"

int dirwalk(const char *dirpath, 
            int (*fn)(const char *filepath, const struct stat *st, void*), void *arg){
    DIR *dir = opendir(dirpath);

    if(dir == NULL){
        perror("opendir");
        return -1;
    }

    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        size_t child_path_size = strlen(dirpath) + strlen(entry->d_name) + 2;
        char *child_path = calloc(child_path_size, sizeof(char));
        if(child_path == NULL){
            perror("calloc");
            closedir(dir);
            return -1;
        }
        snprintf(child_path, child_path_size, "%s/%s", dirpath, entry->d_name);

        struct stat st;
        if(lstat(child_path, &st) == -1){
            perror("lstat");
            free(child_path);
            closedir(dir);
            return -1;
        }

        int callback_return = fn(child_path, &st, arg);
        if(callback_return == -1) {
            free(child_path);
            closedir(dir);
            return callback_return;
        }else if(callback_return == DIR_SKIPCHD) {
            free(child_path);
            continue;
        }

        if(S_ISDIR(st.st_mode)){
            int ret = dirwalk(child_path, fn, arg);
            if(ret != 0) {
                free(child_path);
                closedir(dir);
                return ret;
            }
        }

        free(child_path);
    }

    closedir(dir);
    return 0;
}

int is_our_link(const char *source, const char *destination){
    char target[4096];

    ssize_t n = readlink(destination, target, sizeof(target) - 1);
    if(n == -1) return 0;

    target[n] = '\0';

    return strcmp(target, source) == 0;
}

void remove_trailing_slash(char *path){
    if(path == NULL) return;
    size_t len = strlen(path);

    while (len > 1 && path[len - 1] == '/') {
        path[len - 1] = '\0';
        len -= 1;
    }
}
