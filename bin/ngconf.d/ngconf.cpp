#include <getopt.h>

#include <iostream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;

using std::string;

#define CSTRSIZE 10

#define __GET_SUBOPTS \
    (getsubopt(&subopt.arg, token, &subopt.value))

#define __GET_OPTLONG \
    (getopt_long(argc, argv, opt.scope, options, &subopt.index))

#define __CHECK_SUBOPT \
    (check_subopt(*subopt.arg, errfnd))

#define __CHECK_SUBOPT_ARGS                                      \
    ((optarg == NULL && optind < argc && argv[optind][0] != '-') \
         ? (bool)(optarg = argv[optind++])                       \
         : (optarg != NULL))

#define __IF_SUBOPT_VAL(n)    \
    if (subopt.value == NULL) \
        usage_subopt(token[n], &errfnd);

#define __CHECK_SUBOPT_DIR \
    __IF_SUBOPT_VAL(SUBOPT_DIR)
#define __CHECK_SUBOPT_NAME \
    __IF_SUBOPT_VAL(SUBOPT_NAME)

typedef enum
{
    LONGOPT_HTTPS,
    LONGOPT_INDEX,
    LONGOPT_LOCATION,
    LONGOPT_LOG,
    LONGOPT_NAME,
    LONGOPT_PHP,
    LONGOPT_PORT,
    LONGOPT_ROOT,
    LONGOPT_SAN,
} LONGOPTS;

typedef enum
{
    SUBOPT_DIR,
    SUBOPT_NAME,
    SUBOPT_OTHER,
} SUBOPTS;

int usage(const char *argv0);
int usage_subopt(const char *subopt, int *err);
static inline int check_subopt(const char &c, const int err);
static inline int check_subopt_arg(char **optarg, const int argc, int *optind, char *argv[]);

int main(int argc, char *argv[])
{
    static struct option options[] = {
        {"https", optional_argument, 0, 0},
        {"index", required_argument, 0, 0},
        {"location", required_argument, 0, 0},
        {"log", optional_argument, 0, 0},
        {"name", required_argument, 0, 0},
        {"php", required_argument, 0, 0},
        {"port", required_argument, 0, 0},
        {"root", required_argument, 0, 0},
        {"san", required_argument, 0, 0},
        NULL,
    };
    struct
    {
        int index;
        const char *scope;
    } opt = {
        index : 0,
        scope : "",
    };

    struct
    {
        char name[CSTRSIZE];
    } suboptions[] = {
        {"dir"},
        {"name"},
        {"other"},
    };

    char *const token[] = {
        suboptions[SUBOPT_DIR].name,
        suboptions[SUBOPT_NAME].name,
        suboptions[SUBOPT_OTHER].name,
        NULL,
    };
    struct subopt
    {
        int index;
        char *arg;
        char *value;
    } subopt{
        index : 0,
    };

    int port = 80;
    string index;
    string name = "localhost";
    string root;
    string san;

    struct subopt_fs
    {
        bool active;
        string dir;
        string name;
    };
    struct subopt_fs https = {
        .active = false,
    };
    struct subopt_fs log = {
        .active = false,
    };

    int errfnd = 0;
    while ((opt.index = __GET_OPTLONG) != -1)
    {
        int this_option_optind = optind ? optind : 1;
        switch (opt.index)
        {
        case 0:
            switch (subopt.index)
            {
            case LONGOPT_HTTPS:
                cout << optind << endl;
                if (check_subopt_arg(&optarg, argc, &optind, argv))
                // if (__CHECK_SUBOPT_ARGS)
                {
                    cout << optind << endl;
                    cout << argv[optind] << endl;
                    subopt.arg = optarg;
                    while (__CHECK_SUBOPT)
                    {
                        switch (__GET_SUBOPTS)
                        {
                        case SUBOPT_DIR:
                            __CHECK_SUBOPT_DIR
                            https.dir = subopt.value;
                            break;
                        case SUBOPT_NAME:
                            __CHECK_SUBOPT_NAME
                            https.name = subopt.value;
                            break;
                        }
                    }
                }
                https.active = true;
                break;
            case LONGOPT_INDEX:
                index = optarg;
                break;
            case LONGOPT_LOCATION:
                break;
            case LONGOPT_LOG:
                if (__CHECK_SUBOPT_ARGS)
                {
                    subopt.arg = optarg;
                    while (__CHECK_SUBOPT)
                    {
                        switch (__GET_SUBOPTS)
                        {
                        case SUBOPT_DIR:
                            __CHECK_SUBOPT_DIR
                            log.dir = subopt.value;
                            break;
                        case SUBOPT_NAME:
                            __CHECK_SUBOPT_NAME
                            log.name = subopt.value;
                            break;
                        }
                    }
                }
                log.active = true;
                break;
            case LONGOPT_NAME:
                break;
            case LONGOPT_PHP:
                break;
            case LONGOPT_PORT:
                port = atoi(optarg);
                break;
            case LONGOPT_ROOT:
                root = optarg;
                break;
            case LONGOPT_SAN:
                san = optarg;
                break;
            }
            break;
        default:
            usage(argv[0]);
        }
    }
    // cout << "name : " << name << endl;
    // cout << "san: " << san << endl;
    // cout << "https: " << https.active << " " << https.dir << " " << https.name << endl;
    // cout << "index: " << index << endl;
    // cout << "port: " << port << endl;
    // cout << "log: " << log.active << " " << log.dir << " " << log.name << endl;

    if (optind < argc)
    {
        cout << "non-option ARGV-elements: " << endl;
        while (optind < argc)
            cout << argv[optind++];
        cout << endl;
    }

    return 0;
}

int usage(const char *argv0)
{
    cout << "Usage: "
         << argv0
         << endl;

    exit(1);
}

int usage_subopt(const char *subopt, int *err)
{
    cout << "Usage: --option [" << subopt << "= ] " << endl;
    *err = 1;
    exit(1);
}

static inline int check_subopt(const char &c, const int err)
{
    return (c != '\0' && !err);
}

static inline int check_subopt_arg(char **optarg, const int argc, int *optind, char *argv[])
{
    return (*optarg == NULL && *optind < argc && argv[*optind][0] != '-')
               ? (bool)(*optarg = argv[(*optind)++])
               : (*optarg != NULL);
}