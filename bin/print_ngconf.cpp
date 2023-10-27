#include <getopt.h>

#include <iostream>
#include <sstream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;

using std::istringstream;
using std::string;

#define SIZE_ARR 10
#define SIZE_STR 10

#define CHECK_OPT_ARG check_opt_arg(argc, argv, optind, optarg)
#define CHECK_SUBOPT check_subopt(*subopt, err)
#define CHECK_SUBOPT_ARG(LONGOPT) check_subopt_arg(tokens[LONGOPT], subopt_arg, err)
#define CHECK_SUBOPT_CONF CHECK_SUBOPT_ARG(SUBOPT_CONF)
#define CHECK_SUBOPT_FILENAME CHECK_SUBOPT_ARG(SUBOPT_FILENAME)
#define CHECK_SUBOPT_OTHER CHECK_SUBOPT_ARG(SUBOPT_OTHER)
#define CHECK_SUBOPT_PATH CHECK_SUBOPT_ARG(SUBOPT_PATH)
#define CHECK_SUBOPT_TYPE CHECK_SUBOPT_ARG(SUBOPT_TYPE)
#define CHECK_SUBOPT_VAL CHECK_SUBOPT_ARG(SUBOPT_VAL)
#define GET_LONGOPT getopt_long(argc, argv, scope, options, &longopt)
#define GET_SUBOPT getsubopt(&subopt, tokens, &subopt_arg)

int check_opt_arg(const int argc, char *argv[], int &optind, char *(&optarg));
int check_subopt(const char &c, const int e);
void check_subopt_arg(const char *const token, const char *subopt_arg, int &e);
void usage(const char *argv0);
void usage_subopt(const char *subopt);

int main(int argc, char *argv[])
{
    struct suboption
    {
        char name[SIZE_STR];
    };
    struct Server
    {
        struct Https
        {
            bool active;
            string conf;
            string file_name;
            string path;
            string get_conf()
            {
                return "include " + conf + "; ";
            }
            string get_crt()
            {
                return "ssl_certificate " + path + "/" + file_name + ".crt; ";
            }
            string get_key()
            {
                return "ssl_certificate_key " + path + "/" + file_name + ".key; ";
            }
            string get_ssl()
            {
                return active ? " ssl; " : "; ";
            }
            string get_block()
            {
                return "http2 on; " + get_crt() + get_key() + get_conf();
            }
        };
        struct Domain
        {
            string subject;
            string alternate;
            string get_san()
            {
                return !alternate.empty() ? " " + alternate + "; " : "; ";
            }
            string get_line()
            {
                return "server_name " + subject + get_san();
                ;
            }
        };
        struct Log
        {
            bool active;
            const string *file_name;
            string path;
            string get_access()
            {
                return "access_log " + path + "/" + *file_name + ".access.log; ";
            }
            string get_error()
            {
                return "error_log " + path + "/" + *file_name + ".error.log; ";
            }
            string get_block()
            {
                return get_access() + get_error();
            }
        };
        struct Location
        {
            bool active = false;
            string conf = "";
            string path = "/";
            string type = "try_files";
            string val = "index.html";
            string other[SIZE_ARR];
            string get_conf()
            {
                return !conf.empty() ? "include " + conf + "; " : "";
            }
            string get_typeval()
            {
                return !type.empty() ? type + " " + val + "; " : "";
            }
            string get_other()
            {
                string str;
                for (auto &o : other)
                {
                    if (o.empty())
                        break;
                    str += o + "; ";
                }
                return str;
            }
            string get_block()
            {
                return "location " + path + " { " + get_typeval() + get_other() + get_conf() + "} ";
            }
        };
        string port;
        string index;
        string root;
        struct Domain domain;
        struct Https https;
        struct Log log;
        struct Location locations[SIZE_ARR];
        void print()
        {
            cout << port << " " << root << endl;
            cout << "server { "
                 << "listen " + port + https.get_ssl()
                 << (log.active ? log.get_block() : "")
                 << (https.active ? https.get_block() : "")
                 << domain.get_line()
                 << (!root.empty() ? "root " + root + "; " : "")
                 << (!index.empty() ? "index " + index + "; " : "");
            for (auto &location : locations)
            {
                if (location.active == 0)
                    break;
                cout << location.get_block();
            }
            cout << "}" << endl;
        }
    };
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
    while (1)
    {
        static const char *scope = "i:l::n:p:r:";
        static struct option options[] = {
            {"index", required_argument, 0, 'i'},
            {"location", optional_argument, 0, 'l'},
            {"name", required_argument, 0, 'n'},
            {"port", required_argument, 0, 'p'},
            {"root", required_argument, 0, 'r'},
            {"https", optional_argument, 0, 0},
            {"log", optional_argument, 0, 0},
            {"san", required_argument, 0, 0},
            NULL,
        };
        static struct suboption suboptions[] = {
            {"conf"},
            {"file_name"},
            {"other"},
            {"path"},
            {"type"},
            {"val"},
        };
        enum LONGOPTS
        {
            LONGOPT_HTTPS = 5,
            LONGOPT_LOG,
            LONGOPT_SAN,
        };
        enum SUBOPTS
        {
            SUBOPT_CONF,
            SUBOPT_FILENAME,
            SUBOPT_OTHER,
            SUBOPT_PATH,
            SUBOPT_TYPE,
            SUBOPT_VAL,
        };
        static char *const tokens[] = {
            suboptions[SUBOPT_CONF].name,
            suboptions[SUBOPT_FILENAME].name,
            suboptions[SUBOPT_OTHER].name,
            suboptions[SUBOPT_PATH].name,
            suboptions[SUBOPT_TYPE].name,
            suboptions[SUBOPT_VAL].name,
            NULL,
        };
        static int opt;
        static int longopt;

        static char *subopt;
        static char *subopt_arg;

        opt = GET_LONGOPT;
        if (opt == -1)
            break;
        switch (opt)
        {
        case 0:
            switch (longopt)
            {
            case LONGOPT_HTTPS:
                if (CHECK_OPT_ARG)
                {
                    subopt = optarg;
                    while (CHECK_SUBOPT)
                    {
                        switch (GET_SUBOPT)
                        {
                        case SUBOPT_PATH:
                            CHECK_SUBOPT_PATH;
                            server.https.path = subopt_arg;
                            break;
                        case SUBOPT_FILENAME:
                            CHECK_SUBOPT_FILENAME;
                            server.https.file_name = subopt_arg;
                            break;
                        }
                    }
                }
                server.https.active = true;
                break;
            case LONGOPT_LOG:
                if (CHECK_OPT_ARG)
                {
                    subopt = optarg;
                    while (CHECK_SUBOPT)
                    {
                        switch (GET_SUBOPT)
                        {
                        case SUBOPT_PATH:
                            CHECK_SUBOPT_PATH;
                            server.log.path = subopt_arg;
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
            if (l < SIZE_ARR)
            {
                static int o = 0;
                if (CHECK_OPT_ARG)
                {
                    subopt = optarg;
                    while (CHECK_SUBOPT)
                    {
                        switch (GET_SUBOPT)
                        {
                        case SUBOPT_CONF:
                            CHECK_SUBOPT_CONF;
                            server.locations[l].conf = subopt_arg;
                            break;
                        case SUBOPT_PATH:
                            CHECK_SUBOPT_PATH;
                            server.locations[l].path = subopt_arg;
                            break;
                        case SUBOPT_OTHER:
                            if (o < SIZE_ARR)
                            {
                                CHECK_SUBOPT_OTHER;
                                server.locations[l].other[o++] = subopt_arg;
                            }
                            break;
                        case SUBOPT_TYPE:
                            CHECK_SUBOPT_TYPE;
                            server.locations[l].type = subopt_arg;
                            break;
                        case SUBOPT_VAL:
                            CHECK_SUBOPT_VAL;
                            server.locations[l].val = subopt_arg;
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
        case 'r':
            server.root = optarg;
            break;
        default:
            usage(argv[0]);
        }
    }
    server.print();

    exit(EXIT_SUCCESS);
}

void usage(const char *argv0)
{
    cout << "Usage: "
         << argv0
         << endl;
    exit(EXIT_FAILURE);
}
void usage_subopt(const char *subopt)
{
    cout << "Usage: --option [" << subopt << "= ] " << endl;
    exit(EXIT_FAILURE);
}
int check_opt_arg(const int argc, char *argv[], int &optind, char *(&optarg))
{
    return (optarg == NULL && optind < argc && argv[optind][0] != '-')
               ? (bool)(optarg = argv[optind++])
               : (optarg != NULL);
}
int check_subopt(const char &c, const int e)
{
    return (c != '\0' && !e);
}
void check_subopt_arg(const char *const token, const char *subopt_arg, int &e)
{
    if (subopt_arg == NULL)
    {
        e = 1;
        usage_subopt(token);
    }
};