#include <string.h>
#include "server_cli.h"
#include "server_yml.h"

#define YMLFLAGS                          \
    strcmp("--conf", argv[1]) == 0 ||     \
        strcmp("--yaml", argv[1]) == 0 || \
        strcmp("--yml", argv[1]) == 0 ||  \
        strcmp("-y", argv[1]) == 0 ||     \
        strcmp("--file", argv[1]) == 0 || \
        strcmp("-f", argv[1]) == 0

int main(int argc, char *argv[])
{
    if (YMLFLAGS)
    {
        print_yml(&argv[2]);
    }
    else
    {
        print_cli(argc, argv);
    }
}