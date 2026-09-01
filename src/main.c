#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>

#include "fs.h"
#include "cstow.h"
#include "uncstow.h"

void print_help(){
    printf("Usage: cstow [OPTIONS] ... [PACKAGE] ...\n"
           "A simple to use symlink farm manager\n"
           "\nOPTIONS:\n"
           "  -s, --stow-dir\n\tSpecify the source directory which contain all the packages\n"
           "  -d, --target-dir\n\tSpecify the destination directory which the linking will happen at\n"
           "  -D, --uncstow\n\tRemove the link created in the target directory\n"
           "  -v, --verbose\n\tShow more output for the operation\n");
}

int main(int argc, char *argv[]){
    char *stow_dir, *target_dir;
    int opt;
    int delete = 0;
    struct cstow_cli_options options = {
        .dry_run = 0,
        .verbose = 0
    };

    char cwd[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) == NULL){
        perror("getcwd");
        return 1;
    }

    stow_dir = cwd;

    struct passwd *user = getpwuid(getuid());
    if(user == NULL){
        perror("getpwuid");
        return 1;
    }

    target_dir = user->pw_dir;

    while((opt = getopt(argc, argv, ":hs:d:Dv")) != -1){
        switch(opt){
            case 'h':
                print_help();
                return 0;
            break;
            case 's':
                remove_trailing_slash(optarg);
                stow_dir = optarg;
            break;
            case 'd':
                remove_trailing_slash(optarg);
                target_dir = optarg;
            break;
            case 'D':
                delete = 1;
            break;
            case 'v':
                options.verbose = 1;
            break;
            case '?':
                fprintf(stderr, "Aye bro you might need to use -h or --help\n");
                return 1;
            break;
            case ':':
                fprintf(stderr, "-%c requires an argument but found none\n", optopt);
                return 1;
            break;
        }
    }

    if (argc - optind != 1) {
        fprintf(stderr, "Expected exactly one positional argument\n");
        return 1;
    }

    char *package = argv[optind];
    if(delete) return uncstow(stow_dir, target_dir, package, options);

    return cstow(stow_dir, target_dir, package, options);
}
