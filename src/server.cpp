#include <iostream>
#include <checkopt.h>
#include "server.h"

typedef enum
{
    LONGOPT_INDEX = 0,
    LONGOPT_LOCATION,
    LONGOPT_NAME,
    LONGOPT_PORT,
    LONGOPT_ROOT,
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
void test(int argc, char *argv[])
{
    std::cout << argc << std::endl;
    for (size_t i = 0; i < argc; i++)
    {
        std::cout << argv[i] << std::endl;
    }

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
            file_name : "site",
            path : "/path/to/cert",
        },
        log : {
            active : false,
            file_name : &server.domain.subject,
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
            {"location", 2, 0, 'l'},
            {"name", 1, 0, 'n'},
            {"port", 1, 0, 'p'},
            {"root", 1, 0, 'r'},
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
                            server.https.file_name = suboptarg;
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
            if (l < SIZE_ARR)
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
                            if (o < SIZE_ARR)
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

// Declare usage functions
void usage(const char *argv0)
{
    std::cout << "Usage: "
              << argv0
              << std::endl;
    exit(EXIT_FAILURE);
}
void usage_subopt(const char *subopt)
{
    std::cout << "Usage: --option [" << subopt << "= ] " << std::endl;
    exit(EXIT_FAILURE);
}

// Declare Https methods
void Https::get_conf()
{
    std::cout << "include " << conf << "; ";
}
void Https::get_crt()
{
    std::cout << "ssl_certificate " << path << "/" << file_name << ".crt; ";
}
void Https::get_key()
{
    std::cout << "ssl_certificate_key " << path << "/" << file_name << ".key; ";
}

void Https::get_block()
{
    if (active)
    {
        std::cout << "http2 on; ";
        get_crt();
        get_key();
        get_conf();
    }
}

// Declare Domain methods
void Domain::get_san()
{
    if (*alternate != '\0')
        std::cout << " " << alternate;
}
void Domain::get_line()
{
    std::cout << "server_name " << subject;
    get_san();
    std::cout << "; ";
}

// Declare Location methods
void Location::get_conf()
{
    if (*conf != '\0')
        std::cout << "include " << conf << "; ";
}
void Location::get_typeval()
{
    if (*type != '\0')
        std::cout << type << " " << val << "; ";
}
void Location::get_other()
{
    for (auto &o : other)
    {
        if (o == __null)
            break;
        std::cout << o << "; ";
    }
}
void Location::get_block()
{
    std::cout << "location " << path << " { ";
    get_typeval();
    get_other();
    get_conf();
    std::cout << "} ";
}

// Declare Log methods
void Log::get_access()
{
    std::cout << "access_log " << path << "/" << *file_name << ".access.log; ";
}
void Log::get_error()
{
    std::cout << "error_log " << path << "/" << *file_name << ".error.log; ";
}
void Log::get_block()
{
    if (active)
    {
        get_access();
        get_error();
    }
}

// Declare Server methods
void Server::get_port_line()
{
    std::cout << "listen " << port;
    if (https.active)
        std::cout << " ssl";
    std::cout << "; ";
}
void Server::get_root_line()
{
    if (*root != '\0')
        std::cout << "root " << root << "; ";
}
void Server::get_index_line()
{
    if (*index != '\0')
        std::cout << "index " << index << "; ";
}
void Server::print()
{
    std::cout << "server { ";
    get_port_line();
    domain.get_line();
    https.get_block();
    log.get_block();
    get_root_line();
    get_index_line();
    for (auto &location : locations)
    {
        if (location.active == 0)
            break;
        location.get_block();
    }
    std::cout << "}";
    std::cout << std::endl;
}