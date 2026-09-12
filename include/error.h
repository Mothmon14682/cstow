#ifndef __CSTOW_ERROR_H
#define __CSTOW_ERROR_H

#include <limits.h>

enum cstow_error_code {
    CSTOW_ERR_MISSING_VAL,
    CSTOW_ERR_INVALID_OPT,
    CSTOW_ERR_MISSING_PACK,
    CSTOW_ERR_INTERNAL
};

struct cstow_error {
    enum cstow_error_code code;
    int sys_errno;
    char facility[256];
    char operation[256];

    char src[PATH_MAX];
    char dest[PATH_MAX];
};

void cstow_error_set(struct cstow_error *error,
                            enum cstow_error_code code, int sys_errno, const char *facility, 
                            const char *operation, 
                            const char *src, const char *dest);

void cstow_error_print(const char *facility, struct cstow_error err);

#endif
