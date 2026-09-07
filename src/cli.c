#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <pwd.h>

#include "cli.h"
#include "cstow_types.h"
#include "fs.h"

static void print_help(){
    printf("Usage: cstow [OPTIONS] ... [PACKAGE] ...\n"
           "A simple to use symlink farm manager\n"
           "\nOPTIONS:\n"
           "  -s, --stow-dir\n\tSpecify the source directory which contain all the packages\n\tWill default to the current working directory if not provided\n"
           "  -d, --target-dir\n\tSpecify the destination directory which the linking will happen at\n\tWill default to the $HOME directory if not provived\n"
           "  -D, --uncstow\n\tRemove the link created in the target directory\n"
           "  -v, --verbose\n\tShow more output for the operation\n"
           "  -n, --dry-run\n\tDo not perform any action on filesystem. Only output the actions that will be done\n");
}

static int cstow_cli_default(struct cstow_cli_options *options){
    char cwd[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) == NULL){
        perror("getcwd");
        return -1;
    }

    int needed = snprintf(options->stow_dir, sizeof(options->stow_dir), "%s", cwd);
    if(needed < 0 || needed >= PATH_MAX) return -1;

    struct passwd *user = getpwuid(getuid());
    if(user == NULL){
        perror("getpwuid");
        return -1;
    }

    needed = snprintf(options->target_dir, sizeof(options->target_dir), "%s", user->pw_dir);
    if(needed < 0 || needed >= PATH_MAX) return -1;
    options->op = CSTOW_OP;

    return 0;
}

int cstow_cli_flags_handle(int argc, char *argv[], struct cstow_cli_options *options){
    struct option long_options[] = {
        {"help",       no_argument,       NULL, 'h'},
        {"stow-dir",   required_argument, NULL, 's'},
        {"target-dir", required_argument, NULL, 'd'},
        {"uncstow",    no_argument,       NULL, 'D'},
        {"verbose",    no_argument,       NULL, 'v'},
        {"dry-run",    no_argument,       NULL, 'n'},
        {0, 0, 0, 0}
    };
    int opt;

    if(cstow_cli_default(options) != 0) return -1;

    while((opt = getopt_long(argc, argv, ":hs:d:Dvn", long_options, NULL)) != -1){
        switch(opt){
            case 'h':
                print_help();
                return 0;
            break;
            case 's':
                remove_trailing_slash(optarg);
                strncpy(options->stow_dir, optarg, PATH_MAX);
            break;
            case 'd':
                remove_trailing_slash(optarg);
                strncpy(options->target_dir, optarg, PATH_MAX);
            break;
            case 'D':
                options->op = UNCSTOW_OP;
            break;
            case 'v':
                options->verbose = 1;
            break;
            case 'n':
                options->dry_run = 1;
            break;
            case '?':
                return '?';
            break;
            case ':':
                return ':';
            break;
        }
    }

    return 0;
}
