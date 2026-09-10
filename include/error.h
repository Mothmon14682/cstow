#ifndef __CSTOW_ERROR_H
#define __CSTOW_ERROR_H

#include <limits.h>

enum cstow_error_code {
    CSTOW_ERR_MISSING_VAL,
    CSTOW_ERR_INVALID_OPT,
    CSTWP_ERR_INTERNAL
};

struct cstow_error {
    enum cstow_error_code code;
    int sys_errno;
    char operation[256];

    char src[PATH_MAX];
    char dest[PATH_MAX];
};

inline void cstow_error_set(struct cstow_error *error,
                            enum cstow_error_code code, int sys_errno, 
                            const char *operation, 
                            const char *src, const char *dest);

#endif
