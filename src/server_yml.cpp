#include <yaml-cpp/yaml.h>
#include "stdlib.h"
#include "server.h"
#include "server_yml.h"

#define ASTR as<std::string>()

void print_yml(char **file)
{
    if (**file == '\0')
        exit(1);

    const YAML::Node &config = YAML::LoadFile(*file);
    struct Server servers[MAXARR];
    for (int n = 0; n < config.size(); n++)
    {
        const YAML::Node &node = config[n]["server"];

        if (!node["port"] || !node["domain"]["subject"] ||
            node["port"].IsNull() || node["domain"]["subject"].IsNull())
            exit(1);

        servers[n].active = true;
        servers[n].port = node["port"].ASTR;
        servers[n].domain.subject = node["domain"]["subject"].ASTR;

        if (node["index"] && !node["index"].IsNull())
            servers[n].index = node["index"].ASTR;

        if (node["root"] && !node["root"].IsNull())
            servers[n].root = node["root"].ASTR;

        if (node["log"] && !node["log"].IsNull())
        {
            servers[n].log.active = true;
            if (node["log"].IsMap())
                if (node["log"]["path"] && !node["log"]["path"].IsNull())
                    servers[n].log.path = node["log"]["path"].ASTR;
        }

        if (node["https"] && !node["https"].IsNull())
        {
            servers[n].https.active = true;
            if (node["https"].IsMap())
            {
                if (node["https"]["path"] && !node["https"]["path"].IsNull())
                    servers[n].https.path = node["https"]["path"].ASTR;

                if (node["https"]["filename"] && !node["https"]["filename"].IsNull())
                    servers[n].https.filename = node["https"]["filename"].ASTR;

                if (node["https"]["conf"] && !node["https"]["conf"].IsNull())
                    servers[n].https.conf = node["https"]["conf"].ASTR;
            }
        }
        if (node["locations"] && node["locations"].size())
        {
            for (int l = 0; l < node["locations"].size(); l++)
            {
                servers[n].locations[l].active = 1;
                if (node["locations"][l]["location"]["conf"] &&
                    !node["locations"][l]["location"]["conf"].IsNull())
                    servers[n].locations[l].conf = node["locations"][l]["location"]["conf"].ASTR;

                if (node["locations"][l]["location"]["path"] &&
                    !node["locations"][l]["location"]["path"].IsNull())
                    servers[n].locations[l].path = node["locations"][l]["location"]["path"].ASTR;

                if (node["locations"][l]["location"]["type"] &&
                    !node["locations"][l]["location"]["type"].IsNull())
                    servers[n].locations[l].type = node["locations"][l]["location"]["type"].ASTR;

                if (node["locations"][l]["location"]["val"] &&
                    !node["locations"][l]["location"]["val"].IsNull())
                    servers[n].locations[l].val = node["locations"][l]["location"]["val"].ASTR;

                if (node["locations"][l]["location"]["other"] &&
                    node["locations"][l]["location"]["other"].size())
                    for (int i = 0; i < node["locations"][l]["location"]["other"].size(); i++)
                        if (!node["locations"][l]["location"]["other"][i].IsNull())
                            servers[n].locations[l].other[i] = node["locations"][l]["location"]["other"][i].ASTR;
            }
        }
        for (auto &server : servers)
        {
            if (server.active != 1)
                break;
            server.print();
        }
    }
}
