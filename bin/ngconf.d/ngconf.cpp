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
#define CHECK_SUBOPT_DIR CHECK_SUBOPT_ARG(SUBOPT_DIR)
#define CHECK_SUBOPT_NAME CHECK_SUBOPT_ARG(SUBOPT_NAME)
#define CHECK_SUBOPT_OTHER CHECK_SUBOPT_ARG(SUBOPT_OTHER)
#define CHECK_SUBOPT_PROP CHECK_SUBOPT_ARG(SUBOPT_PROP)
#define GET_LONGOPT getopt_long(argc, argv, scope, options, &longopt)
#define GET_SUBOPT getsubopt(&subopt, tokens, &subopt_arg)

typedef enum
{
    LONGOPT_HTTPS = 5,
    LONGOPT_LOG,
    LONGOPT_PHP_FASTCGI,
    LONGOPT_SAN,
} LONGOPTS;
typedef enum
{
    SUBOPT_CONF,
    SUBOPT_DIR,
    SUBOPT_NAME,
    SUBOPT_OTHER,
    SUBOPT_PROP,
} SUBOPTS;

struct suboption
{
    char name[SIZE_STR];
    int value;
};

struct LOpt
{
    string name;
    string value;
};
struct Location
{
    string route;
    struct LOpt conf;
    struct LOpt prop;
    string other[SIZE_ARR];
    string get_property()
    {
        return prop.name.empty() ? prop.name + " " + prop.value : NULL;
    };
};

struct Fs
{
    bool active;
    string dir;
    string name;
};

struct Server
{
    int port;
    string index;
    string name;
    string root;
    string san;
    struct Fs https;
    struct Fs log;
    struct Location locations[SIZE_ARR];
};

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

int main(int argc, char *argv[])
{
    struct Server server = {
        port : 80,
        index : "",
        name : "localhost",
        root : "",
        san : "",
        https : {
            active : false,
            dir : "/path/to/certs",
            name : "site",
        },
        log : {
            active : false,
            dir : "/var/log/nginx",
            name : "default",
        },
        locations : {},
    };

    int err = 0;
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
            {"php_fastcgi", required_argument, 0, 0},
            {"san", required_argument, 0, 0},
            NULL,
        };
        static struct suboption suboptions[] = {
            {"conf"},
            {"dir"},
            {"name"},
            {"other"},
            {"prop"},
        };

        static char *const tokens[] = {
            suboptions[SUBOPT_CONF].name,
            suboptions[SUBOPT_DIR].name,
            suboptions[SUBOPT_NAME].name,
            suboptions[SUBOPT_OTHER].name,
            suboptions[SUBOPT_PROP].name,
            NULL,
        };

        static int opt;
        static int longopt;
        static int location_index = 0;

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
                        case SUBOPT_DIR:
                            CHECK_SUBOPT_DIR;
                            server.https.dir = subopt_arg;
                            break;
                        case SUBOPT_NAME:
                            CHECK_SUBOPT_NAME;
                            server.https.name = subopt_arg;
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
                        case SUBOPT_DIR:
                            CHECK_SUBOPT_DIR;
                            server.log.dir = subopt_arg;
                            break;
                        case SUBOPT_NAME:
                            CHECK_SUBOPT_NAME;
                            server.log.name = subopt_arg;
                            break;
                        }
                    }
                }
                server.log.active = true;
                break;
            case LONGOPT_PHP_FASTCGI:
                break;
            case LONGOPT_SAN:
                server.san = optarg;
                break;
            }
            break;
        case 'i':
            server.index = optarg;
            break;
        case 'l':
            if (location_index < SIZE_ARR)
            {
                static int other_index = 0;
                if (CHECK_OPT_ARG)
                {
                    subopt = optarg;
                    while (CHECK_SUBOPT)
                    {
                        switch (GET_SUBOPT)
                        {
                        case SUBOPT_CONF:
                            CHECK_SUBOPT_CONF;
                            server.locations[location_index].conf.value = subopt_arg;
                            break;
                        case SUBOPT_DIR:
                            CHECK_SUBOPT_DIR;
                            server.locations[location_index].route = subopt_arg;
                            break;
                        case SUBOPT_OTHER:
                            if (other_index < SIZE_ARR)
                            {
                                CHECK_SUBOPT_OTHER;
                                server.locations[location_index].other[other_index++] = subopt_arg;
                            }
                            break;
                        case SUBOPT_PROP:
                            CHECK_SUBOPT_PROP;
                            server.locations[location_index].prop.value = subopt_arg;

                            break;
                        }
                    }
                }
                location_index++;
            }
            break;
        case 'n':
            server.name = optarg;
            break;
        case 'p':
        {
            istringstream stream(optarg);
            stream >> server.port;
        }
        case 'r':
            server.root = optarg;
            break;
        default:
            usage(argv[0]);
        }
    }
    cout << "name : " << server.name << endl;
    cout << "san: " << server.san << endl;
    cout << "root: " << server.root << endl;
    cout << "https: " << server.https.active << " " << server.https.dir << " " << server.https.name << endl;
    cout << "index: " << server.index << endl;
    cout << "port: " << server.port << endl;
    cout << "log: " << server.log.active << " " << server.log.dir << " " << server.log.name << endl;
    cout << "location conf: " << server.locations[0].conf.name << " " << server.locations[0].conf.value << "\n"
         << "location other: " << server.locations[0].other[0] << "\n";

    // if (optind < argc)
    // {
    //     cout << "non-option ARGV-elements: " << endl;
    //     while (optind < argc)
    //         cout << argv[optind++];
    //     cout << endl;
    // }

    exit(EXIT_SUCCESS);
}
