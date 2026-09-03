#ifndef __CSTOW_LINK_MANAGER_H
#define __CSTOW_LINK_MANAGER_H

#include "fs.h"
#include "planner.h"

enum cstow_operation{
    CSTOW_OP,
    UNCSTOW_OP
};

struct cstow_context {
    const char *target_dir;
    const char *package_dir;

    struct cstow_cli_options options;
    enum cstow_operation op;
    struct cstow_planner *planner;
};

int link_manager_action(const char* stowdir, const char* target_dir, const char* package, struct cstow_cli_options options, enum cstow_operation op);

#endif
