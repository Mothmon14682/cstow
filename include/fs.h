#ifndef __CSTOW_FS_H
#define __CSTOW_FS_H

#define DIR_SKIPCHD 1

enum {
    PROCESS_ERROR = -1,
    PROCESS_SUCCESS = 0,
    PROCESS_CREATED_LINK = 1
};

int dirwalk(const char *dirpath, int (*fn)(const char *filepath, const struct stat *st, void*), void *ctx);
int cstow_process_path(const char* source, const char* destination);
int uncstow_process_path(const char* source, const char* destination);

#endif
