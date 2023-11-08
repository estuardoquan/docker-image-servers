#include "checkopt.h"

int check_optarg(const int &argc, char **(&argv), int &optind, char *(&optarg))
{
    return (optarg == __null && optind < argc && argv[optind][0] != '-')
               ? (bool)(optarg = argv[optind++])
               : (optarg != __null);
}
int check_subopt(const char &c, const int &e)
{
    return (c != '\0' && !e);
}
void check_suboptarg(const char *suboptarg, int &e, void (*callback)(const char *), const char *const &token)
{
    if (suboptarg == __null)
        e = 1;
    callback(token);
}