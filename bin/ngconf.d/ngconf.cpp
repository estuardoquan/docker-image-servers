#include <getopt.h>
#include <stdlib.h>

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
using std::oct;
using std::string;

#define __CHECK_OPTARG                                           \
    ((optarg == NULL && optind < argc && argv[optind][0] != '-') \
         ? (bool)(optarg = argv[optind++])                       \
         : (optarg != NULL))

typedef enum
{
    SUBOPT_DIR,
    SUBOPT_NAME,
    SUBOPT_OTHER,
} __SUBOPTS;

typedef struct __cstruct
{
    char value[10];
} __cstruct;

typedef struct __subopt
{
    char *args;
    char *value;
} __subopt;

typedef struct __fopt
{
    bool active;
    string dir;
    string name;
} __https;

static int usage(const char *argv0)
{
    cout << "Usage: "
         << argv0
         << endl;

    return 1;
}

int main(int argc, char *argv[])
{
    string scope = "";
    int opt;
    int digit_optind = 0;

    static struct option long_options[] = {
        {"name", required_argument, 0, 0},
        {"san", required_argument, 0, 0},
        {"https", optional_argument, 0, 0},
        {"port", required_argument, 0, 0},
        {"root", required_argument, 0, 0},
        {"index", required_argument, 0, 0},
        {"log", optional_argument, 0, 0},
        {"location", required_argument, 0, 0},
        {"php", required_argument, 0, 0},
        {0, 0, 0, 0},
    };

    static struct __cstruct sub_options[] = {
        [SUBOPT_DIR] = {"dir"},
        [SUBOPT_NAME] = {"name"},
        [SUBOPT_OTHER] = {"other"},
    };

    char *const token[] = {
        [SUBOPT_DIR] = sub_options[SUBOPT_DIR].value,
        [SUBOPT_NAME] = sub_options[SUBOPT_NAME].value,
        [SUBOPT_OTHER] = sub_options[SUBOPT_OTHER].value,
        NULL,
    };
    __subopt subopt;
    int errfnd = 0;

    string name = "localhost";
    string san;
    __fopt https = {
        .active = false,
    };
    int port = 80;
    string root;
    string index;
    __fopt log = {
        .active = false,
    };

    while (1)
    {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;

        opt = getopt_long(argc, argv, scope.c_str(),
                          long_options, &option_index);
        if (opt == -1)
            break;

        switch (opt)
        {
        case 0:
            if (long_options[option_index].name == "name")
            {
                cout << "option " << long_options[option_index].name;
                if (optarg)
                    cout << " with arg " << optarg;
                cout << endl;
            }
            if (long_options[option_index].name == "san")
            {
                if (san.empty())
                {
                    san = optarg;
                }
                else
                {
                    san = san + " " + optarg;
                }
            }
            if (long_options[option_index].name == "https")
            {
                https.active = true;
                if (__CHECK_OPTARG)
                {
                    subopt.args = optarg;
                    while (*subopt.args != '\0' && !errfnd)
                    {
                        switch (getsubopt(&subopt.args, token, &subopt.value))
                        {
                        case SUBOPT_DIR:
                            if (subopt.value == NULL)
                            {
                                cout << "Missing value for suboption "
                                     << token[SUBOPT_DIR]
                                     << endl;
                                errfnd = 1;
                                continue;
                            }
                            https.dir = subopt.value;
                            break;
                        case SUBOPT_NAME:
                            if (subopt.value == NULL)
                            {
                                cout << "Missing value for suboption "
                                     << token[SUBOPT_NAME]
                                     << endl;
                                errfnd = 1;
                                continue;
                            }
                            https.name = subopt.value;
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
            if (long_options[option_index].name == "port")
            {
                port = atoi(optarg);
            }
            if (long_options[option_index].name == "root")
            {
                root = optarg;
            }
            if (long_options[option_index].name == "index")
            {
                index = optarg;
            }
            if (long_options[option_index].name == "log")
            {
                log.active = true;
                if (__CHECK_OPTARG)
                {
                    subopt.args = optarg;
                    while (*subopt.args != '\0' && !errfnd)
                    {
                        switch (getsubopt(&subopt.args, token, &subopt.value))
                        {
                        case SUBOPT_DIR:
                            if (subopt.value == NULL)
                            {
                                cout << "Missing value for suboption "
                                     << token[SUBOPT_DIR]
                                     << endl;
                                errfnd = 1;
                                continue;
                            }
                            log.dir = subopt.value;
                            break;
                        case SUBOPT_NAME:
                            if (subopt.value == NULL)
                            {
                                cout << "Missing value for suboption "
                                     << token[SUBOPT_NAME]
                                     << endl;
                                errfnd = 1;
                                continue;
                            }
                            log.name = subopt.value;
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
            if (long_options[option_index].name == "location")
            {
                // TODO
            }
            if (long_options[option_index].name == "php")
            {
                // TODO
            }
            break;
        default:
            cout << "opt returned char code " << opt << "/0" << oct << opt << endl;
            usage(argv[0]);
        }
    }
    cout << "name : " << name << endl;
    cout << "san: " << san << endl;
    cout << "https: " << https.active << " " << https.dir << " " << https.name << endl;
    cout << "port: " << port << endl;
    cout << "log: " << log.active << " " << log.dir << " " << log.name << endl;
    return 0;
}