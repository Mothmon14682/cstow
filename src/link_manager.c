#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>
#include <libgen.h>

#include "fs.h"
#include "planner.h"
#include "link_manager.h"

static int cstow_process_path(const char* source, const char* destination, const struct stat *st_source, struct cstow_context *ctx){
    if(source == NULL || destination == NULL) return PROCESS_ERROR;

    struct stat st_dest;
    int verbose = ctx->options.verbose;

    if (lstat(destination, &st_dest) == -1) {
        if (errno != ENOENT) {
            perror("lstat destination");
            return PROCESS_ERROR;
        }

        if (cstow_planner_add(ctx->planner, CSTOW_ACTION_CREATE, source, destination) != 0) return PROCESS_ERROR;

        if(S_ISDIR(st_source->st_mode)) { 
            if(verbose) fprintf(stdout, "cstow: Skipped the children of %s directory\n", source);

            return PROCESS_SKIPCHD; 
        }

        return PROCESS_SUCCESS;
    }

    if (S_ISDIR(st_dest.st_mode) && S_ISDIR(st_source->st_mode)) {
        if(verbose) fprintf(stdout, "cstow: The directory in %s is user created at %s so not create link\n", source, destination);

        return PROCESS_SUCCESS;
    }

    if (S_ISLNK(st_dest.st_mode)) {
        if (is_our_link(source, destination)) {
            if(S_ISDIR(st_source->st_mode)){
                if(verbose) fprintf(stdout, "cstow: %s is a link to directory at %s so skip creat link for the children of the directory\n", destination, source);

                return PROCESS_SKIPCHD;
            }

            if(verbose) fprintf(stdout, "cstow: A link existed at %s so not create link\n", destination);

            return PROCESS_SUCCESS;
        }

        fprintf(stderr, "conflict: %s exists but not from cstow\n", destination);
        return PROCESS_ERROR;
    }

    fprintf(stderr, "conflict: %s exists but not from cstow\n", destination);
    return PROCESS_ERROR;
}

static int uncstow_process_path(const char* source, const char* destination, const struct stat *st_source, struct cstow_context *ctx){
    if(source == NULL || destination == NULL) return PROCESS_ERROR;

    struct stat st_dest;
    int verbose = ctx->options.verbose;

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

        if (cstow_planner_add(ctx->planner, CSTOW_ACTION_REMOVE, source, destination) != 0) return PROCESS_ERROR;

        if(S_ISDIR(st_source->st_mode)){
            if(verbose) fprintf(stdout, "uncstow: Skipping the children of %s directory\n", source);

            return PROCESS_SKIPCHD;
        }

        return PROCESS_SUCCESS;
    }

    if (S_ISDIR(st_dest.st_mode)) {
        if(verbose) fprintf(stdout, "uncstow: %s is a directory so not unlink or remove it\n", destination);

        return PROCESS_SUCCESS;
    }

    fprintf(stderr, "conflict: %s\n", destination);
    return PROCESS_ERROR;
}

static int link_manager_callback(const char* filepath, const struct stat *st, void *arg){
    struct cstow_context *ctx = arg;
    if(ctx == NULL || st == NULL) return -1;

    const char *relative = filepath + strlen(ctx->package_dir);
    if (*relative == '/')
        relative++;

    char destination[PATH_MAX];

    int needed = snprintf(destination, sizeof(destination), "%s/%s", ctx->target_dir, relative);
    if(needed < 0 || needed >= PATH_MAX){
        fprintf(stderr, "Path name too long\n");
        return -1;
    }

    if(ctx->op == CSTOW_OP){
        int process_status = cstow_process_path(filepath, destination, st, ctx);
        switch (process_status) {
            case PROCESS_ERROR:  return -1;
            case PROCESS_SKIPCHD: return DIR_SKIPCHD;
        }
    }else if(ctx->op == UNCSTOW_OP){
        int process_status = uncstow_process_path(filepath, destination, st, ctx);
        switch (process_status) {
            case PROCESS_ERROR:  return -1;
            case PROCESS_SKIPCHD: return DIR_SKIPCHD;
        }
    }
    return 0;
}

int link_manager_action(const char* stowdir, const char* target_dir, const char* package, struct cstow_cli_options options, enum cstow_operation op){
    char real_stowdir[PATH_MAX];
    char real_target_dir[PATH_MAX];

    if(realpath(stowdir, real_stowdir) == NULL){
        perror("realpath");
        return -1;
    }

    if(realpath(target_dir, real_target_dir) == NULL){
        perror("realpath");
        return -1;
    }

    char package_dir[PATH_MAX];

    int needed = snprintf(package_dir, sizeof(package_dir), "%s/%s", real_stowdir, package);
    if(needed < 0 || needed >= PATH_MAX) return -1;

    struct cstow_planner planner;
    cstow_planner_init(&planner);

    struct cstow_context ctx = {
        .target_dir = real_target_dir,
        .package_dir = package_dir,
        .options = options,
        .op = op,
        .planner = &planner
    };

    return dirwalk(package_dir, link_manager_callback, &ctx);
}
