#ifndef __CSTOW_TYPES_H
#define __CSTOW_TYPES_H

#include <limits.h>
enum cstow_operation{
    CSTOW_OP,
    UNCSTOW_OP
};

struct cstow_cli_options{
    int verbose;
    int dry_run;
    char stow_dir[PATH_MAX];
    char target_dir[PATH_MAX];

    enum cstow_operation op;
};

#endif
