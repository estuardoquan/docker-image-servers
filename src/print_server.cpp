#include <stdio.h>
#include <stdlib.h>
#include <options.h>
#include "print_server.h"

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
    exit(EXIT_FAILURE);
}

void usage_subopt(const char *subopt)
{
    printf("Usage: --option[ %s= ]\n", subopt);
    exit(EXIT_FAILURE);
}

void Server::print()
{
    printf("server \n{\n");
    printf("\tlisten %s%s;\n", port, https.active ? " ssl" : "");

    name = domain.name();
    printf("\tserver_name %s;\n\n", name);
    free(name);

    if (log.active)
    {
        printf("\taccess_log %s/%s.access.log;\n", log.path, *log.filename);
        printf("\terror_log %s/%s.access.log;\n\n", log.path, *log.filename);
    }

    if (https.active)
    {
        printf("\tssl_certificate %s/%s.crt;\n", https.path, https.filename);
        printf("\tssl_certificate_key %s/%s.key;\n", https.path, https.filename);
        printf("\tinclude %s;\n\n", https.conf);
    }
    if (*root != '\0')
        printf("\troot %s;\n\n", root);
    if (*index != '\0')
        printf("\tindex %s;\n\n", index);
    for (Location &location : locations)
    {
        if (location.active == 0)
            break;
        printf("\tlocation %s\n\t{\n", location.path);
        if (*location.type != '\0')
            printf("\t\t%s %s;\n", location.type, location.val);
        if (*location.conf != '\0')
            printf("\t\tinclude %s;\n", location.conf);
        for (const char *(&o) : location.other)
        {
            if (o == __null)
                break;
            printf("\t\t%s;\n", o);
        }
        printf("\t}\n\n");
    }
    printf("}\n");
}

void print_server(int argc, char *argv[])
{
    struct Server server = {
        port : "80",
        index : "",
        root : "",
        domain : {
            subject : "localhost",
            alternate : ""
        },
        https : {
            active : false,
            conf : "/etc/nginx/extra/default.https.conf",
            filename : "site",
            path : "/var/local/step",
        },
        log : {
            active : false,
            filename : &server.domain.subject,
            path : "/var/log/nginx",
        },
        locations : {},
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
            {"other"},
            {"path"},
            {"type"},
            {"val"},
        };
        static char *const tokens[] = {
            suboptions[SUBOPT_CONF].name,
            suboptions[SUBOPT_FILENAME].name,
            suboptions[SUBOPT_OTHER].name,
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
char *charstr(const char **a, const char **b)
{
    char *c = (char *)calloc(0, sizeof(char));
    int i, j, n;
    i = j = n = 0;
    while ((*a)[i] != '\0')
    {
        c = (char *)realloc(c, (i * sizeof(char)) + 1);
        c[i] = (*a)[i];
        i++;
    }
    if ((*b)[j] != '\0')
        c[i++] = ' ';
    while ((*b)[j] != '\0')
    {
        c = (char *)realloc(c, (j * sizeof(char)) + 2);
        c[j + i] = (*b)[j];
        j++;
    }
    return c;
}