#include <stdio.h>
#include <stdlib.h>
#include <options.h>

#include <sstream>
#include "server.h"
#include "server_cli.h"

typedef enum
{
    OPT_NAME,
    OPT_PORT,
    OPT_HTTPS,
    OPT_LOCATION,
    OPT_REDIRECT,
    OPT_INDEX,
    OPT_LOG,
    OPT_ROOT,
    OPT_SAN,
} OPTS;

typedef enum
{
    SUBOPT_FILENAME,
    SUBOPT_INCLUDE,
    SUBOPT_OTHER,
    SUBOPT_PATH,
    SUBOPT_TYPE,
    SUBOPT_VALUE,
} SUBOPTS;

void print_cli(int argc, char **(&argv))
{
    struct Server server = {
        port : 80,
        domain : {
            subject : "localhost",
        },
    };

    int err = 0;
    int l = 0;
    int o = 0;
    while (1)
    {
        const char *scope = "n:p:s:l:r:";

        static struct option options[] = {
            {"name", 1, 0, 'n'},
            {"port", 1, 0, 'p'},
            {"https", 1, 0, 's'},
            {"location", 1, 0, 'l'},
            {"redirect", 1, 0, 'r'},
            {"index", 1, 0, 0},
            {"log", 1, 0, 0},
            {"root", 1, 0, 0},
            {"san", 1, 0, 0},
            __null,
        };
        static struct suboption suboptions[] = {
            {"filename"},
            {"include"},
            {"other"},
            {"path"},
            {"type"},
            {"value"},
        };
        static char *const tokens[] = {
            suboptions[SUBOPT_FILENAME].name,
            suboptions[SUBOPT_INCLUDE].name,
            suboptions[SUBOPT_OTHER].name,
            suboptions[SUBOPT_PATH].name,
            suboptions[SUBOPT_TYPE].name,
            suboptions[SUBOPT_VALUE].name,
            __null,
        };

        static int opt;
        static int longopt;

        static char *subopt;
        static char *suboptarg;

        opt = GET_LONGOPT;
        if (opt == -1)
            break;
        switch (opt)
        {
        case 0:
            switch (longopt)
            {
            case OPT_INDEX:
                server.index = optarg;
                break;

                break;
            case OPT_LOG:
                server.log = optarg;
                break;
            case OPT_ROOT:
                server.root = optarg;
                break;
            case OPT_SAN:
                server.domain.alternate = optarg;
                break;
            }
            break;
        case 'l':
            if (l < MAXARR)
            {
                o = 0;
                if (CHECK_OPTARG)
                {
                    subopt = optarg;
                    while (CHECK_SUBOPT)
                    {
                        switch (GET_SUBOPT)
                        {
                        case SUBOPT_INCLUDE:
                            CHECK_SUBOPTARG(SUBOPT_INCLUDE);
                            server.locations[l].include = suboptarg;
                            break;
                        case SUBOPT_PATH:
                            CHECK_SUBOPTARG(SUBOPT_PATH);
                            server.locations[l].path = suboptarg;
                            break;
                        case SUBOPT_OTHER:
                            if (o < MAXARR)
                            {
                                CHECK_SUBOPTARG(SUBOPT_OTHER);
                                server.locations[l].other[o++] = suboptarg;
                            }
                            break;
                        case SUBOPT_TYPE:
                            CHECK_SUBOPTARG(SUBOPT_TYPE);
                            server.locations[l].type = suboptarg;
                            break;
                        case SUBOPT_VALUE:
                            CHECK_SUBOPTARG(SUBOPT_VALUE);
                            server.locations[l].value = suboptarg;
                            break;
                        }
                    }
                }
                server.locations[l++].active = true;
            }
            break;
        case 'n':
            server.domain.subject = optarg;
            break;
        case 'p':
        {
            std::istringstream iss(optarg);
            iss >> server.port;
        }
        break;
        case 'r':
        {
            if (CHECK_OPTARG)
            {
                subopt = optarg;
                while (CHECK_SUBOPT)
                {
                    switch (GET_SUBOPT)
                    {
                    case SUBOPT_OTHER:
                        CHECK_SUBOPTARG(SUBOPT_OTHER);
                        server.redirect.other = suboptarg;
                        break;
                    case SUBOPT_VALUE:
                    {
                        CHECK_SUBOPTARG(SUBOPT_VALUE);
                        std::istringstream iss(suboptarg);
                        iss >> server.redirect.value;
                    }
                    break;
                    default:
                    {
                        std::istringstream iss(subopt);
                        iss >> server.redirect.value;
                    }
                    break;
                    }
                }
            }
            server.redirect.active = true;
        }
        break;
        case 's':
        {
            if (CHECK_OPTARG)
            {
                subopt = optarg;
                while (CHECK_SUBOPT)
                {
                    switch (GET_SUBOPT)
                    {
                    case SUBOPT_FILENAME:
                        CHECK_SUBOPTARG(SUBOPT_FILENAME);
                        server.https.filename = suboptarg;
                        break;
                    case SUBOPT_INCLUDE:
                        CHECK_SUBOPTARG(SUBOPT_INCLUDE);
                        server.https.include = suboptarg;
                        break;
                    case SUBOPT_PATH:
                        CHECK_SUBOPTARG(SUBOPT_PATH);
                        server.https.path = suboptarg;
                        break;
                    default:
                        server.https.path = optarg;
                        break;
                    }
                }
            }
            server.https.active = true;
        }
        break;
        default:
            usage(argv[0]);
            break;
        }
    }
    server.print();
}

void usage(const char *argv)
{
    printf("Usage : %s\n", argv);
    exit(1);
}

void usage_subopt(const char *subopt)
{
    printf("Usage: --option[ %s= ]\n", subopt);
    exit(1);
}
