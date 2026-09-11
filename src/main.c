#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <getopt.h>

#include "cli.h"
#include "cstow_types.h"
#include "error.h"
#include "link_manager.h"

int main(int argc, char *argv[]){
    struct cstow_cli_options options;

    if(cstow_cli_flags_handle(argc, argv, &options) != 0){
        cstow_error_print("cli", options.error); 

        return -1;
    }

    for(int i = optind; i < argc; i++){
        char *package = argv[i];
       
        if(link_manager_action(options.stow_dir, options.target_dir, package, options, options.op) == -1){
            if(options.op == CSTOW_OP) fprintf(stderr, "Failed to cstow package: %s\n", package);
            if(options.op == UNCSTOW_OP) fprintf(stderr, "Failed to uncstow package: %s\n", package);

            return 1;
        }else {
            if(options.op == CSTOW_OP) fprintf(stdout, "Successfully cstow package: %s\n", package);
            if(options.op == UNCSTOW_OP) fprintf(stdout, "Successfully uncstow package: %s\n", package);
        }
    }

    return 0;
}
