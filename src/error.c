#include <string.h>
#include <limits.h>

#include "error.h"

inline void cstow_error_set(struct cstow_error *error,
                            enum cstow_error_code code, int sys_errno, 
                            const char *operation, 
                            const char *src, const char *dest){
    error->code = code;
    error->sys_errno = sys_errno;
    strncpy(error->operation, operation, 256);
    strncpy(error->src, src, PATH_MAX);
    strncpy(error->dest, dest, PATH_MAX);
}
