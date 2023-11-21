#pragma once
#define MAXARR 10

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
    const char **filename;
    const char *path = "/var/log/nginx";
};

struct Location
{

    bool active = false;
    const char *conf = "";
    const char *path = "/";
    const char *type = "try_files";
    const char *val = "index.html";
    // char *other[MAXARR] = {};
};

struct Server
{
    bool active = false;
    const char *port = "";
    const char *index = "";
    const char *root = "";
    char *name;
    struct Domain domain;
    struct Https https;
    struct Log log = {
        filename : &domain.subject,
    };
    struct Location locations[MAXARR] = {};

    void print();
};