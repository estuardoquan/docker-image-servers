#include <string.h>
#include <stdio.h>
#include "server_cli.h"
#include "server_yml.h"

#define YMLFLAGS                          \
    strcmp("--conf", argv[1]) == 0 ||     \
        strcmp("--yaml", argv[1]) == 0 || \
        strcmp("--yml", argv[1]) == 0 ||  \
        strcmp("--file", argv[1]) == 0 || \
        strcmp("-f", argv[1]) == 0

#define CLIFLAGS                          \
    strcmp("make", argv[1]) == 0 ||       \
        strcmp("create", argv[1]) == 0 || \
        strcmp("print", argv[1]) == 0

int main(int argc, char *argv[])
{
    if (argc < 2)
        return 1;
    if (YMLFLAGS)
        if (argc == 3)
            print_yml(&argv[2]);
        else
            return 1;
    else if (CLIFLAGS)
        print_cli(argc, argv);
    else
        return 1;
    return 0;
}