#pragma once
#define SIZE_ARR 10

struct Https
{
    bool active;
    const char *conf;
    const char *file_name;
    const char *path;

    void get_conf();
    void get_crt();
    void get_key();
    void get_ssl();
    void get_block();
};

struct Domain
{
    const char *subject;
    const char *alternate;

    void get_san();
    void get_line();
};

struct Location
{
    bool active = false;
    const char *conf = "";
    const char *path = "/";
    const char *type = "try_files";
    const char *val = "index.html";
    char *other[SIZE_ARR];

    void get_conf();
    void get_typeval();
    void get_other();
    void get_block();
};

struct Log
{
    bool active = false;
    const char **file_name;
    const char *path = "/var/log/nginx";

    void get_access();
    void get_error();
    void get_block();
};

struct Server
{
    const char *port;
    const char *index;
    const char *root;
    struct Domain domain;
    struct Https https;
    struct Log log;
    struct Location locations[SIZE_ARR];

    void print();
    void get_port_line();
    void get_root_line();
    void get_index_line();
};

void test(int argc, char *argv[]);
void usage(const char *argv0);
void usage_subopt(const char *subopt);