#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <getopt.h>

#include "fs.h"
#include "link_manager.h"

struct option long_options[] = {
    {"help",       no_argument,       NULL, 'h'},
    {"stow-dir",   required_argument, NULL, 's'},
    {"target-dir", required_argument, NULL, 'd'},
    {"uncstow",    no_argument,       NULL, 'D'},
    {"verbose",    no_argument,       NULL, 'v'},
    {0, 0, 0, 0}
};

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

    while((opt = getopt_long(argc, argv, ":hs:d:Dv", long_options, NULL)) != -1){
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
    if(delete) return link_manager_action(stow_dir, target_dir, package, options, UNCSTOW_OP);

    return link_manager_action(stow_dir, target_dir, package, options, CSTOW_OP);
}
