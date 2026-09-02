#ifndef __CSTOW_LINK_MANAGER_H
#define __CSTOW_LINK_MANAGER_H

#include "fs.h"

int link_manager_action(const char* stowdir, const char* target_dir, const char* package, struct cstow_cli_options options, enum cstow_operation op);

#endif
