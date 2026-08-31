#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>

#include "fs.h"
#include "uncstow.h"

static int uncstow_process_path(const char* source, const char* destination){
    if(source == NULL || destination == NULL) return PROCESS_ERROR;

    struct stat st_dest;
    struct stat st_source;
    
    if(lstat(source, &st_source) == -1){
        perror("lstat destination");
        return PROCESS_ERROR;
    }

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

        if(S_ISDIR(st_source.st_mode)) return PROCESS_UNLINK_DIR;

        return PROCESS_SUCCESS;
    }

    if (S_ISDIR(st_dest.st_mode)) {
        return PROCESS_SUCCESS;
    }

    fprintf(stderr, "conflict: %s\n", destination);
    return PROCESS_ERROR;
}

static int uncstow_callback(const char *filepath, const struct stat *st, void *arg){
    struct cstow_context *ctx = arg;
    if(ctx == NULL || st == NULL) return -1;

    const char *relative = filepath + strlen(ctx->package_dir);
    if (*relative == '/')
        relative++;

    size_t destination_size = strlen(ctx->target_dir) + strlen(relative) + 2;
    char *destination = malloc(destination_size);
    if (destination == NULL) {
        perror("malloc");
        return -1;
    }
    snprintf(destination, destination_size, "%s/%s", ctx->target_dir, relative);
    
    int process_status = uncstow_process_path(filepath, destination);
    switch (process_status) {
        case PROCESS_ERROR:  
            free(destination);
            return -1;
        break;
        case PROCESS_UNLINK_DIR: 
            if(ctx->options.verbose) printf("Unlinked: %s\n", destination);

            free(destination);
            return DIR_SKIPCHD;
        break;
    }

    if(ctx->options.verbose) printf("Unlinked: %s\n", destination);

    free(destination);
    return 0;
}

int uncstow(const char* stowdir, const char* target_dir, const char* package, struct cstow_cli_options options){
    char package_dir[PATH_MAX];

    int needed = snprintf(package_dir, sizeof(package_dir), "%s/%s", stowdir, package);
    if(needed > PATH_MAX) return -1;

    struct cstow_context ctx = {
        .stow_dir = stowdir,
        .target_dir = target_dir,
        .package = package,
        .package_dir = package_dir,
        .options = options
    };

    return dirwalk(package_dir, uncstow_callback, &ctx);
}
