#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "fs.h"
#include "cstow.h"
#include "uncstow.h"

int main(){
    struct cstow_context ctx = {
        .stow_dir = "/home/tuan/Code/cstow",
        .target_dir = "/home/tuan",
        .package = "test",
        .package_dir = "/home/tuan/Code/cstow/test"
    };

    // cstow(ctx.stow_dir, ctx.target_dir, ctx.package);
    uncstow(ctx.stow_dir, ctx.target_dir, ctx.package);
    return 0;
}
