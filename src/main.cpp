#include <fstream>
#include <yaml-cpp/yaml.h>
#include "print_server.h"

#define AS_CSTR as<std::string>().c_str()

int main(int argc, char *argv[])
{
    const char *file = "/home/estuardoquan/Projects/proxy.d/server/src/config.yaml";

    YAML::Node config = YAML::LoadFile(file);

    struct Server server = {
        port : "",
        index : "",
        root : "",
        domain : {
            subject : "",
            alternate : ""
        },
        https : {
            active : false,
            conf : "/etc/nginx/extra/default.https.conf",
            filename : "site",
            path : "/var/local/step",
        },
        log : {
            active : false,
            filename : &server.domain.subject,
            path : "/var/log/nginx",
        },
        locations : {},
    };
    for (int n = 0; n < config.size(); n++)
    {
        YAML::Node node = config[n]["server"];
        if (!node["port"] || !node["domain"]["subject"] ||
            node["port"].IsNull() || node["domain"]["subject"].IsNull())
            exit(1);
        server.port = node["port"].AS_CSTR;
        server.domain.subject = node["domain"]["subject"].AS_CSTR;

        if (node["index"] && !node["index"].IsNull())
            server.index = node["index"].AS_CSTR;

        if (node["root"] && !node["root"].IsNull())
            server.root = node["root"].AS_CSTR;

        if (node["domain"]["alternate"] && !node["domain"]["alternate"].IsNull())
            server.domain.alternate = node["domain"]["alternate"].AS_CSTR;

        if (node["log"] && !node["log"].IsNull())
        {
            server.log.active = true;
            server.log.path = node["log"].AS_CSTR;
        }

        if (node["https"])
        {
            server.https.active = true;

            if (node["https"]["path"] && !node["https"]["path"].IsNull())
                server.https.path = node["https"]["path"].AS_CSTR;

            if (node["https"]["filename"] && !node["https"]["filename"].IsNull())
                server.https.filename = node["https"]["filename"].AS_CSTR;

            if (node["https"]["conf"] && !node["https"]["conf"].IsNull())
                server.https.conf = node["https"]["conf"].AS_CSTR;
        }
        if (node["locations"].size())
        {
            for (int l = 0; l < node["locations"].size(); l++)
            {
                char *test;
                server.locations[l].active = 1;
                if (node["locations"][l]["location"]["conf"] && !node["locations"][l]["location"]["conf"].IsNull())
                    server.locations[l].conf = node["locations"][l]["location"]["conf"].AS_CSTR;

                if (node["locations"][l]["location"]["path"] && !node["locations"][l]["location"]["path"].IsNull())
                    server.locations[l].path = node["locations"][l]["location"]["path"].AS_CSTR;

                if (node["locations"][l]["location"]["type"] && !node["locations"][l]["location"]["type"].IsNull())
                    server.locations[l].type = node["locations"][l]["location"]["type"].AS_CSTR;

                if (node["locations"][l]["location"]["val"] && !node["locations"][l]["location"]["val"].IsNull())
                    server.locations[l].val = node["locations"][l]["location"]["val"].AS_CSTR;

                for (int i = 0; i < node["locations"][l]["location"]["other"].size(); i++)
                {
                    printf("%i\n", i);
                    if (!node["locations"][l]["location"]["other"][i].IsNull())
                        server.locations[l].other[i] = node["locations"][l]["location"]["other"][i].AS_CSTR;
                    // server.locations[l].other[o] =.AS_CSTR;
                    printf("%s\n", server.locations[l].other[0]);
                }

                // server.locations[l].other[4] = node["locations"][l]["location"]["other"][1].AS_CSTR;
            }
        }
    }
    print_server(argc, argv);
    server.print();
    return 0;
}