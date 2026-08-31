#ifndef __CSTOW_H
#define __CSTOW_H

#include "fs.h"

int cstow(const char* stowdir, const char* target_dir, const char* package, struct cstow_cli_options options);

#endif
