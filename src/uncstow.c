#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#include "fs.h"
#include "uncstow.h"

int uncstow_process_path(const char* source, const char* destination){
    if(source == NULL || destination == NULL) return PROCESS_ERROR;

    struct stat st_dest;
    if(lstat(destination, &st_dest) == -1){
        if (errno == ENOENT) return PROCESS_SUCCESS;

        perror("lstat destination");
        return PROCESS_ERROR;
    }

    if(S_ISLNK(st_dest.st_mode)){
        if (!is_our_link(source, destination)) {
            fprintf(stderr, "conflict: %s is not a link to %s\n", destination, source);
            return PROCESS_ERROR;
        }

        if (unlink(destination) == -1) {
            perror("unlink");
            return PROCESS_ERROR;
        }

        return PROCESS_SUCCESS;
    }

    if (S_ISDIR(st_dest.st_mode)) {
        return PROCESS_SUCCESS;
    }

    fprintf(stderr, "conflict: %s\n", destination);
    return PROCESS_ERROR;
}
