#include "options.h"

int check_optarg(const int argc, char **argv[], int *optind, char **optarg)
{
    return (*optarg == __null && *optind < argc && (*argv)[*optind][0] != '-')
               ? (bool)(*optarg = (*argv)[(*optind)++])
               : (*optarg != __null);
}
int check_subopt(const char c, const int e)
{
    return (c != '\0' && !e);
}
void check_suboptarg(char **suboptarg, int *e, suboption subopt, void (*callback)(const char *))
{
    if (*suboptarg == __null)
    {
        *e = 1;
        if (callback != __null)
            callback;
    }
}
