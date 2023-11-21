#pragma once
#include <string>

#define MAXARR 10

struct CLIServer
{
    struct Domain
    {
        const char *subject = "";
        const char *alternate = "";
        char *name();
    };

    struct Https
    {
        bool active = false;
        const char *conf = "/etc/nginx/extra/default.https.conf";
        const char *filename = "site";
        const char *path = "var/local/step";
    };

    struct Log
    {
        bool active = false;
        const char *path = "/var/log/nginx";
    };

    struct Location
    {

        bool active = false;
        const char *conf = "";
        const char *path = "/";
        const char *type = "try_files";
        const char *val = "index.html";
        char *other[MAXARR] = {};
    };

    bool active = false;
    const char *port = "";
    const char *index = "";
    const char *root = "";
    char *name;
    struct Domain domain;
    struct Https https;
    struct Log log;
    struct Location locations[MAXARR] = {};

    void print();
};

struct YMLServer
{
    struct Domain
    {
        std::string subject = "";
        std::string alternate = "";
    };

    struct Https
    {
        bool active = false;
        std::string conf = "/etc/nginx/extra/default.https.conf";
        std::string filename = "site";
        std::string path = "var/local/step";
    };

    struct Log
    {
        bool active = false;
        std::string path = "/var/log/nginx";
    };

    struct Location
    {

        bool active = false;
        std::string conf = "";
        std::string path = "/";
        std::string type = "try_files";
        std::string val = "index.html";
        std::string other[MAXARR] = {};
    };

    bool active = false;
    std::string port = "";
    std::string index = "";
    std::string root = "";
    struct Domain domain;
    struct Https https;
    struct Log log;
    struct Location locations[MAXARR] = {};

    void print();
};
