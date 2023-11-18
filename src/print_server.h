#pragma once

#define MAXARR 10

void print_server(int argc, char *argv[]);
void usage(const char *argv0);
void usage_subopt(const char *subopt);
char *charstr(const char **a, const char **b);

struct Domain
{
    const char *subject;
    const char *alternate;
    char *name()
    {
        return charstr(&subject, &alternate);
    }
};

struct Https
{
    bool active;
    const char *conf;
    const char *filename;
    const char *path;
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
    const char *other[MAXARR];
};

struct Server
{
    char *name;
    const char *port;
    const char *index;
    const char *root;
    struct Domain domain;
    struct Https https;
    struct Log log;
    struct Location locations[MAXARR];

    void print();
};
