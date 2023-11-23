#pragma once
#include <getopt.h>

#define CHECK_OPTARG check_optarg(argc, &argv, &optind, &optarg)
#define CHECK_SUBOPT check_subopt(*subopt, err)
#define CHECK_SUBOPTARG(SUBOPT) check_suboptarg(&suboptarg, &err, suboptions[SUBOPT], &usage_subopt)
#define GET_LONGOPT getopt_long(argc, argv, scope, options, &longopt)
#define GET_SUBOPT getsubopt(&subopt, tokens, &suboptarg)

struct suboption
{
    char name[10];
};

int check_optarg(const int argc, char **argv[], int *optind, char **optarg);
int check_subopt(const char c, const int e);
void check_suboptarg(char **suboptarg, int *e, suboption subopt, void (*callback)(const char *));
