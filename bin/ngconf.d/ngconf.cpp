#define _XOPEN_SOURCE 500

#include <assert.h>
#include <getopt.h>
#include <stdlib.h>

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
using std::oct;
using std::string;

static int usage(const char *argv0)
{
    cout << "Usage: "
         << argv0
         << endl;

    return 1;
}

struct Https
{
    bool active;
    string dir;
    string name;
};

int main(int argc, char *argv[])
{

    string scope = "n:s:h:p:r:i:l:";
    int opt;
    int digit_optind = 0;

    string name;
    string san;
    enum HTTPS_OPTS
    {
        HTTPS_DIR_OPT,
        HTTPS_NAME_OPT,
    };
    char https_name[] = "name";
    char https_dir[] = "dir";
    char *const https_token[] = {
        https_dir,
        https_name,
        NULL,
    };
    char *https_subopts;
    char *https_value;
    int https_errfnd = 0;
    Https https;
    int port = 80;
    string root;
    string index;
    string log;

    while (1)
    {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"name", required_argument, 0, 0},
            {"san", required_argument, 0, 0},
            {"https", required_argument, 0, 0},
            {"port", required_argument, 0, 0},
            {"root", required_argument, 0, 0},
            {"index", required_argument, 0, 0},
            {"log", required_argument, 0, 0},
            {"location", required_argument, 0, 0},
            {"php", required_argument, 0, 0},
            {0, 0, 0, 0}};

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
                if (optarg)
                {
                    https_subopts = optarg;
                    while (*https_subopts != '\0' && !https_errfnd)
                    {
                        switch (getsubopt(&https_subopts, https_token, &https_value))
                        {
                        case HTTPS_DIR_OPT:
                            if (https_value == NULL)
                            {
                                cout << "Missing value for suboption "
                                     << https_token[HTTPS_NAME_OPT]
                                     << endl;
                                https_errfnd = 1;
                                continue;
                            }
                            https.dir = https_value;
                            break;
                        case HTTPS_NAME_OPT:
                            if (https_value == NULL)
                            {
                                cout << "Missing value for suboption "
                                     << https_token[HTTPS_NAME_OPT]
                                     << endl;
                                https_errfnd = 1;
                                continue;
                            }
                            https.name = https_value;
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
            if (long_options[option_index].name == "port")
            {
                // TODO
            }
            if (long_options[option_index].name == "root")
            {
                // TODO
            }
            if (long_options[option_index].name == "index")
            {
                // TODO
            }
            if (long_options[option_index].name == "log")
            {
                // TODO
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
    cout << san << endl;
    cout << https.active << " " << https.dir << " " << https.name << endl;
    return 0;
}