#pragma once

#define SIZE_ARR 10
#define SIZE_STR 10

struct Https
{
    bool active;
    std::string conf;
    std::string file_name;
    std::string path;
    std::string get_conf();
    std::string get_crt();
    std::string get_key();
    std::string get_ssl();
    std::string get_block();
};
struct Domain
{
    std::string subject;
    std::string alternate;
    std::string get_san();
    std::string get_line();
};
struct Location
{
    bool active = false;
    std::string conf = "";
    std::string path = "/";
    std::string type = "try_files";
    std::string val = "index.html";
    std::string other[10];
    std::string get_conf();
    std::string get_typeval();
    std::string get_other();
    std::string get_block();
};
struct Log
{
    bool active = false;
    const std::string *file_name;
    std::string path = "/var/log/nginx";
    std::string get_access();
    std::string get_error();
    std::string get_block();
};
struct Server
{
    int l = 0;
    std::string port;
    std::string index;
    std::string root;
    struct Domain domain;
    struct Https https;
    struct Log log;
    struct Location locations[10];
    void print();
};

void usage(const char *argv0);
void usage_subopt(const char *subopt);