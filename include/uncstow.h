#ifndef __CSTOW_UNCSTOW_H
#define __CSTOW_UNCSTOW_H

#include "fs.h"

int uncstow(const char* stowdir, const char* target_dir, const char* package, struct cstow_cli_options options);

#endif
