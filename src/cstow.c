#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#include "fs.h"
#include "cstow.h"

int cstow_process_path(const char* source, const char* destination){
    if(source == NULL || destination == NULL) return PROCESS_ERROR;

    struct stat st_dest;
    struct stat st_source;

    if(lstat(source, &st_source) == -1){
        perror("lstat source");
        return PROCESS_ERROR;
    }

    if (lstat(destination, &st_dest) == -1) {
        if (errno != ENOENT) {
            perror("lstat destination");
            return PROCESS_ERROR;
        }

        if (symlink(source, destination) == -1) {
            perror("symlink");
            return PROCESS_ERROR;
        }

        return PROCESS_CREATED_LINK;
    }

    if (S_ISDIR(st_dest.st_mode) && S_ISDIR(st_source.st_mode)) {
        return PROCESS_SUCCESS;
    }

    if (S_ISLNK(st_dest.st_mode)) {
        if (is_our_link(source, destination)) return PROCESS_SUCCESS;

        fprintf(stderr, "conflict: %s exists but not from cstow\n", destination);
        return PROCESS_ERROR;
    }

    fprintf(stderr, "conflict: %s exists but not from cstow\n", destination);
    return PROCESS_ERROR;
}
