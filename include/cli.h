#ifndef __CSTOW_CLI_H
#define __CSTOW_CLI_H

#include "cstow_types.h"

int cstow_cli_flags_handle(int argc, char *argv[], struct cstow_cli_options *options);
int cstow_cli_package_handle(int argc, char *argv[], int optind, struct cstow_cli_options *options);

#endif 
