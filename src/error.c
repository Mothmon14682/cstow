#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "error.h"

static char *cstow_error_str(enum cstow_error_code code, int errno){
    switch (code) {
        case CSTOW_ERR_MISSING_VAL: return "An option is missing value";
        case CSTOW_ERR_INVALID_OPT: return "The provided option is not supported";
        case CSTOW_ERR_MISSING_PACK: return "No package was provided";
        case CSTOW_ERR_INTERNAL: return strerror(errno);
    }

    return "INVALID";
}

void cstow_error_set(struct cstow_error *error,
                            enum cstow_error_code code, int sys_errno, const char *facility,
                            const char *operation, 
                            const char *src, const char *dest){
    

    error->code = code;
    error->sys_errno = sys_errno;
    strncpy(error->facility, facility, 256);
    strncpy(error->operation, operation, 256);

    if(src != NULL) strncpy(error->src, src, PATH_MAX);
    else error->src[0] = '\0';

    if(dest != NULL) strncpy(error->dest, dest, PATH_MAX);
    else error->dest[0] = '\0';
}

void cstow_error_print(const char *facility, struct cstow_error error){
    fprintf(stderr, "\x1B[41m\x1B[1m ERROR \x1B[0m\x1B[47m\x1B[30m\x1B[1m %s: %s \x1B[0m \x1B[31m %s \x1B[0m", 
            facility, 
            error.operation, 
            cstow_error_str(error.code, error.sys_errno));

    if(strlen(error.src) != 0) printf("\nsource: %s", error.src);
    if(strlen(error.dest) != 0) printf("\ndestination: %s", error.dest);
    printf("\n");
}
