#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>
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
        char child_path[PATH_MAX];

        int needed = snprintf(child_path, sizeof(child_path), "%s/%s", dirpath, entry->d_name);
        if(needed < 0 || needed >= PATH_MAX){
            fprintf(stderr, "Path name too long\n");
            closedir(dir);
            return -1;
        }

        struct stat st;
        if(lstat(child_path, &st) == -1){
            perror("lstat");
            closedir(dir);
            return -1;
        }

        int callback_return = fn(child_path, &st, arg);
        if(callback_return == -1) {
            closedir(dir);
            return callback_return;
        }else if(callback_return == DIR_SKIPCHD) {
            continue;
        }

        if(S_ISDIR(st.st_mode)){
            int ret = dirwalk(child_path, fn, arg);
            if(ret != 0) {
                closedir(dir);
                return ret;
            }
        }
    }

    closedir(dir);
    return 0;
}

int is_our_link(const char *source, const char *destination){
    char target[PATH_MAX];

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
