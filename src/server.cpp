#include <iostream>
#include <string>
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

int test(int argc, char *argv[])
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

    while (1)
    {
        int err = 0;
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
            if (server.l < SIZE_ARR)
            {
                static int o = 0;
                if (CHECK_OPTARG)
                {
                    subopt = optarg;
                    while (CHECK_SUBOPT)
                    {
                        switch (GET_SUBOPT)
                        {
                        case SUBOPT_CONF:
                            CHECK_SUBOPTARG(SUBOPT_CONF);
                            server.locations[server.l].conf = suboptarg;
                            break;
                        case SUBOPT_PATH:
                            CHECK_SUBOPTARG(SUBOPT_PATH);
                            server.locations[server.l].path = suboptarg;
                            break;
                        case SUBOPT_OTHER:
                            if (o < SIZE_ARR)
                            {
                                CHECK_SUBOPTARG(SUBOPT_OTHER);
                                server.locations[server.l].other[o++] = suboptarg;
                            }
                            break;
                        case SUBOPT_TYPE:
                            CHECK_SUBOPTARG(SUBOPT_TYPE);
                            server.locations[server.l].type = suboptarg;
                            break;
                        case SUBOPT_VAL:
                            CHECK_SUBOPTARG(SUBOPT_VAL);
                            server.locations[server.l].val = suboptarg;
                            break;
                        }
                    }
                }
                server.locations[server.l++].active = true;
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
    exit(EXIT_SUCCESS);
}

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
std::string Https::get_conf()
{
    return "include " + conf + "; ";
}
std::string Https::get_crt()
{
    return "ssl_certificate " + path + "/" + file_name + ".crt; ";
}
std::string Https::get_key()
{
    return "ssl_certificate_key " + path + "/" + file_name + ".key; ";
}
std::string Https::get_ssl()
{
    return active ? " ssl; " : "; ";
}
std::string Https::get_block()
{
    return active ? "http2 on; " + get_crt() + get_key() + get_conf() : "";
}

// Declare Domain methods
std::string Domain::get_san()
{
    return !alternate.empty() ? " " + alternate + "; " : "; ";
}
std::string Domain::get_line()
{
    return "server_name " + subject + get_san();
    ;
}

// Declare Location methods
std::string Location::get_conf()
{
    return !conf.empty() ? "include " + conf + "; " : "";
}
std::string Location::get_typeval()
{
    return !type.empty() ? type + " " + val + "; " : "";
}
std::string Location::get_other()
{
    std::string str;
    for (auto &o : other)
    {
        if (o.empty())
            break;
        str += o + "; ";
    }
    return str;
}
std::string Location::get_block()
{
    return "location " + path + " { " + get_typeval() + get_other() + get_conf() + "} ";
}

// Declare Log methods
std::string Log::get_access()
{
    return "access_log " + path + "/" + *file_name + ".access.log; ";
}
std::string Log::get_error()
{
    return "error_log " + path + "/" + *file_name + ".error.log; ";
}
std::string Log::get_block()
{
    return active ? get_access() + get_error() : "";
}

// Declare Server methods
void Server::print()
{
    std::cout << "server { "
              << "listen " + port + https.get_ssl()
              << https.get_block()
              << domain.get_line()
              << log.get_block()
              << (!root.empty() ? "root " + root + "; " : "")
              << (!index.empty() ? "index " + index + "; " : "");
    for (auto &location : locations)
    {
        if (location.active == 0)
            break;
        std::cout << location.get_block();
    }
    std::cout << "}" << std::endl;
}