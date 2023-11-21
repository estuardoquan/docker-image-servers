#include <stdio.h>
#include <stdlib.h>
#include <options.h>

#include "server.h"
#include "server_cli.h"

typedef enum
{
    LONGOPT_INDEX = 0,
    LONGOPT_NAME,
    LONGOPT_PORT,
    LONGOPT_ROOT,
    LONGOPT_LOCATION,
    LONGOPT_HTTPS,
    LONGOPT_LOG,
    LONGOPT_SAN,
} OPTS;

typedef enum
{
    SUBOPT_CONF = 0,
    SUBOPT_FILENAME,
    SUBOPT_OTHER,
    SUBOPT_PATH,
    SUBOPT_TYPE,
    SUBOPT_VAL,
} SUBOPTS;

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

void print_cli(int argc, char **(&argv))
{
    struct Server server = {
        port : "80",
        domain : {
            subject : "localhost",
        },
    };

    int err = 0;
    int l = 0;
    int o = 0;
    while (1)
    {
        const char *scope = "i:l::n:p:r:";

        static struct option options[] = {
            {"index", 1, 0, 'i'},
            {"name", 1, 0, 'n'},
            {"port", 1, 0, 'p'},
            {"root", 1, 0, 'r'},
            {"location", 2, 0, 'l'},
            {"https", 2, 0, 0},
            {"log", 2, 0, 0},
            {"san", 1, 0, 0},
            __null,
        };
        static struct suboption suboptions[] = {
            {"conf"},
            {"file_name"},
            // {"other"},
            {"path"},
            {"type"},
            {"val"},
        };
        static char *const tokens[] = {
            suboptions[SUBOPT_CONF].name,
            suboptions[SUBOPT_FILENAME].name,
            // suboptions[SUBOPT_OTHER].name,
            suboptions[SUBOPT_PATH].name,
            suboptions[SUBOPT_TYPE].name,
            suboptions[SUBOPT_VAL].name,
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
            case LONGOPT_HTTPS:
                if (CHECK_OPTARG)
                {
                    subopt = optarg;
                    while (CHECK_SUBOPT)
                    {
                        switch (GET_SUBOPT)
                        {
                        case SUBOPT_PATH:
                            CHECK_SUBOPTARG(SUBOPT_PATH);
                            server.https.path = suboptarg;
                            break;
                        case SUBOPT_FILENAME:
                            CHECK_SUBOPTARG(SUBOPT_FILENAME);
                            server.https.filename = suboptarg;
                            break;
                        }
                    }
                }
                server.https.active = true;
                break;
            case LONGOPT_LOG:
                if (CHECK_OPTARG)
                {
                    subopt = optarg;
                    while (CHECK_SUBOPT)
                    {
                        switch (GET_SUBOPT)
                        {
                        case SUBOPT_PATH:
                            CHECK_SUBOPTARG(SUBOPT_PATH);
                            server.log.path = suboptarg;
                            break;
                        }
                    }
                }
                server.log.active = true;
                break;
            case LONGOPT_SAN:
                server.domain.alternate = optarg;
                break;
            }
            break;
        case 'i':
            server.index = optarg;
            break;
        case 'l':
            o = 0;
            if (l < MAXARR)
            {
                if (CHECK_OPTARG)
                {
                    subopt = optarg;
                    while (CHECK_SUBOPT)
                    {
                        switch (GET_SUBOPT)
                        {
                        case SUBOPT_CONF:
                            CHECK_SUBOPTARG(SUBOPT_CONF);
                            server.locations[l].conf = suboptarg;
                            break;
                        case SUBOPT_PATH:
                            CHECK_SUBOPTARG(SUBOPT_PATH);
                            server.locations[l].path = suboptarg;
                            break;
                        // case SUBOPT_OTHER:
                        //     if (o < MAXARR)
                        //     {
                        //         CHECK_SUBOPTARG(SUBOPT_OTHER);
                        //         server.locations[l].other[o++] = suboptarg;
                        //     }
                        //     break;
                        case SUBOPT_TYPE:
                            CHECK_SUBOPTARG(SUBOPT_TYPE);
                            server.locations[l].type = suboptarg;
                            break;
                        case SUBOPT_VAL:
                            CHECK_SUBOPTARG(SUBOPT_VAL);
                            server.locations[l].val = suboptarg;
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
            server.port = optarg;
            break;
        case 'r':
            server.root = optarg;
            break;
        default:
            usage(argv[0]);
            break;
        }
    }
    server.print();
}