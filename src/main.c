#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <getopt.h>

#include "cli.h"
#include "cstow_types.h"
#include "error.h"

int main(int argc, char *argv[]){
    struct cstow_cli_options options;

    if(cstow_cli_flags_handle(argc, argv, &options) != 0){
        cstow_error_print("cli", options.error); 

        return 1;
    }

    if(cstow_cli_package_handle(argc, argv, optind, &options) != 0){
        cstow_error_print("cli", options.error); 

        return 1;
    }
    return 0;
}
