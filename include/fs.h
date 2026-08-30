#ifndef __CSTOW_FS_H
#define __CSTOW_FS_H

#define DIR_SKIPCHD 1

enum {
    PROCESS_ERROR = -1,
    PROCESS_SUCCESS = 0,
    PROCESS_CREATED_LINK = 1
};

struct cstow_context {
    const char *stow_dir;
    const char *target_dir;
    const char *package;
    const char *package_dir;
};

int dirwalk(const char *dirpath, int (*fn)(const char *filepath, const struct stat *st, void*), void *ctx);
int is_our_link(const char *source, const char *destination);
void remove_trailing_slash(char *path);

#endif
