#include <getopt.h>

#include <iostream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;

using std::string;

#define CSTRSIZE 10

#define __GET_SUBOPTS \
    (getsubopt(&subopt.cstr, token, &subopt.value))

#define __SET_OPT \
    (opt = getopt_long(argc, argv, scope, options, &o))

#define __CHECK_SUBOPT \
    (check_subopt(*subopt.cstr, &errfnd))

#define __CHECK_SUBOPT_ARGS                                      \
    ((optarg == NULL && optind < argc && argv[optind][0] != '-') \
         ? (bool)(optarg = argv[optind++])                       \
         : (optarg != NULL))

#define __IF_SUBOPT_VAL(n)    \
    if (subopt.value == NULL) \
        subopt_usage(token[n], &errfnd);

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

typedef struct cstruct
{
    char cstr[CSTRSIZE];
} cstruct;

struct subopt
{
    char *cstr;
    char *value;
};

struct fsopt
{
    bool active;
    string dir;
    string name;
};

int usage(const char *argv0)
{
    cout << "Usage: "
         << argv0
         << endl;

    exit(1);
}

int subopt_usage(const char *subopt, int *err)
{
    cout << "Usage: --option [" << subopt << "= ] " << endl;
    *err = 1;
    exit(1);
}

int check_subopt(const char &c, int *err)
{
    return (c != '\0' && !*err);
}

int main(int argc, char *argv[])
{
    const char *scope = "";
    int opt;
    struct subopt subopt;
    int errfnd = 0;
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

    static cstruct suboptions[] = {
        {"dir"},
        {"name"},
        {"other"},
    };

    char *const token[] = {
        suboptions[SUBOPT_DIR].cstr,
        suboptions[SUBOPT_NAME].cstr,
        suboptions[SUBOPT_OTHER].cstr,
        NULL,
    };

    string name = "localhost";
    string san;
    struct fsopt https = {
        .active = false,
    };
    int port = 80;
    string root;
    string index;
    struct fsopt log = {
        .active = false,
    };

    int o = 0;
    while ((__SET_OPT) != -1)
    {
        switch (opt)
        {
        case 0:
            switch (o)
            {
            case LONGOPT_HTTPS:
                https.active = true;
                if (__CHECK_SUBOPT_ARGS)
                {
                    subopt.cstr = optarg;
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
                break;
            case LONGOPT_INDEX:
                index = optarg;
                break;
            case LONGOPT_LOCATION:
                break;
            case LONGOPT_LOG:
                log.active = true;
                if (__CHECK_SUBOPT_ARGS)
                {
                    subopt.cstr = optarg;
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
                san = san.empty() ? optarg : san + " " + optarg;
                break;
            }
            break;
        default:
            usage(argv[0]);
        }
    }
    cout << "name : " << name << endl;
    cout << "san: " << san << endl;
    cout << "https: " << https.active << " " << https.dir << " " << https.name << endl;
    cout << "port: " << port << endl;
    cout << "log: " << log.active << " " << log.dir << " " << log.name << endl;

    if (optind < argc)
    {
        cout << "non-option ARGV-elements: " << endl;
        while (optind < argc)
            cout << argv[optind++];
        cout << endl;
    }

    return 0;
}