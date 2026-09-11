#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "error.h"

void cstow_error_set(struct cstow_error *error,
                            enum cstow_error_code code, int sys_errno, 
                            const char *operation, 
                            const char *src, const char *dest){
    

    error->code = code;
    error->sys_errno = sys_errno;
    strncpy(error->operation, operation, 256);

    if(src != NULL) strncpy(error->src, src, PATH_MAX);
    else error->src[0] = '\0';

    if(dest != NULL) strncpy(error->dest, dest, PATH_MAX);
    else error->dest[0] = '\0';
}

void cstow_error_print(const char *facility, struct cstow_error error){
    fprintf(stderr, "\x1B[41m\x1B[1m ERROR \x1B[0m\x1B[47m\x1B[30m\x1B[1m %s: %s \x1B[0m", facility, error.operation);
    if(error.sys_errno != 0) printf("\x1B[31m %s \x1B[0m", strerror(error.sys_errno));
    if(strlen(error.src) != 0) printf("\nsource: %s", error.src);
    if(strlen(error.dest) != 0) printf("\ndestination: %s", error.dest);
    printf("\n");
}
