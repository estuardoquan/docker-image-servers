#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>

#include "print_server.h"

using std::cout;
using std::endl;
using std::string;

using namespace YAML;

int main(int argc, char *argv[])
{
    const char *file = "/home/estuardoquan/Projects/proxy.d/server/src/config.yaml";

    Node config = LoadFile(file);

    Node server = config["server"];

    Node port = server["port"];
    Node index = server["index"];
    Node root = server["root"];
    Node log = server["log"];

    Node domain = server["domain"];
    Node https = server["https"];
    Node locations = server["locations"];

    cout << "\n(int)\t\tport:\t" << port.as<int>() << endl;
    cout << "(string)\tindex:\t" << index.as<string>() << endl;
    cout << "(string)\troot:\t" << root.as<string>() << endl;

    cout << "\n"
         << "domain[" << domain.size() << "]" << endl;
    cout << "(string)\tdomain.sub:\t" << domain["subject"].as<string>() << endl;
    cout << "(string)\tdomain.alt:\t" << domain["alternate"].as<string>() << endl;

    cout << "\n"
         << "https[" << https.size() << "]" << endl;
    cout << "(string)\thttps.conf:\t" << https["conf"].as<string>() << endl;
    cout << "(string)\thttps.path:\t" << https["path"].as<string>() << endl;
    cout << "(string)\thttps.filename:\t" << https["filename"].as<string>() << endl;

    cout << "\n"
         << "locations[" << locations.size() << "]" << endl;
    cout << "(string)\tlocations[1].conf:\t" << locations["location"]["conf"].as<string>() << endl;
    cout << "(string)\tlocations[1].path:\t" << locations["location"]["path"].as<string>() << endl;
    cout << "(string)\tlocations[1].type:\t" << locations["location"]["type"].as<string>() << endl;
    cout << "(string)\tlocations[1].val:\t" << locations["location"]["val"].as<string>() << endl;
    // cout << "(string)\tlocations[1].other:\t" << locations[1]["other"] << endl;

    // if (node["domain"])

    // {
    //     Domain domain = node["domain"].as<Domain>();
    //     std::cout << domain.subject << domain.alternate << std::endl;
    // }
    // if (node["https"])
    // {
    //     Https https = node["https"].as<Https>();
    //     printf("%s", https.path);
    // }

    print_server(argc, argv);
    return 0;
}