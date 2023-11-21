#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server_cli.h"
#include "server_yml.h"

#define YMLCMP strcmp("yml", argv[1]) == 0 || strcmp("yaml", argv[1]) == 0

int main(int argc, char *argv[])
{
    if (YMLCMP)
    {
        print_yml(&argv[2]);
    }
    else
    {
        print_cli(argc, argv);
    }
}