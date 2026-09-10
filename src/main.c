#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <pwd.h>
#include <getopt.h>

#include "cli.h"
#include "cstow_types.h"
#include "link_manager.h"

int main(int argc, char *argv[]){
    struct cstow_cli_options options;

    if(cstow_cli_flags_handle(argc, argv, &options) != 0){
        printf("error: cli: %s", options.error.operation);
        if(options.error.sys_errno != 0) printf("%s", strerror(options.error.sys_errno));
        if(strlen(options.error.src) != 0) printf("\nsource: %s", options.error.src);
        if(strlen(options.error.dest) != 0) printf("\ndestination: %s", options.error.dest);
        printf("\n");

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
