#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "fs.h"
#include "cstow.h"
#include "uncstow.h"

int uncstow_callback(const char *filepath, const struct stat *st, void *arg){
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
    if(process_status == PROCESS_ERROR) return -1;

    free(destination);
    return 0;
}

int main(){
    struct cstow_context ctx = {
        .stow_dir = "/home/tuan/Code/cstow",
        .target_dir = "/home/tuan",
        .package = "test",
        .package_dir = "/home/tuan/Code/cstow/test"
    };

    // cstow(ctx.stow_dir, ctx.target_dir, ctx.package);
    dirwalk(ctx.package_dir, uncstow_callback, &ctx);
    return 0;
}
