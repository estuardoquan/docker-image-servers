#include <yaml-cpp/yaml.h>

#include "server.h"
#include "server_yml.h"

#define AS_STR as<std::string>()

void print_yml(char **file)
{
    if (**file == '\0')
        exit(1);
    const YAML::Node &config = YAML::LoadFile(*file);

    struct YMLServer servers[MAXARR];

    for (int n = 0; n < config.size(); n++)
    {
        const YAML::Node &node = config[n]["server"];

        if (!node["port"] || !node["domain"]["subject"] ||
            node["port"].IsNull() || node["domain"]["subject"].IsNull())
            exit(1);
        servers[n].active = true;
        servers[n].port = node["port"].AS_STR;
        servers[n].domain.subject = node["domain"]["subject"].AS_STR;

        if (node["index"] && !node["index"].IsNull())
            servers[n].index = node["index"].AS_STR;

        if (node["root"] && !node["root"].IsNull())
            servers[n].root = node["root"].AS_STR;

        if (node["domain"]["alternate"] && !node["domain"]["alternate"].IsNull())
            servers[n].domain.alternate = node["domain"]["alternate"].AS_STR;

        if (node["log"] && !node["log"].IsNull())
        {
            servers[n].log.active = true;
            servers[n].log.path = node["log"].AS_STR;
        }

        if (node["https"])
        {
            servers[n].https.active = true;

            if (node["https"]["path"] && !node["https"]["path"].IsNull())
                servers[n].https.path = node["https"]["path"].AS_STR;

            if (node["https"]["filename"] && !node["https"]["filename"].IsNull())
                servers[n].https.filename = node["https"]["filename"].AS_STR;

            if (node["https"]["conf"] && !node["https"]["conf"].IsNull())
                servers[n].https.conf = node["https"]["conf"].AS_STR;
        }
        if (node["locations"] && node["locations"].size())
        {
            for (int l = 0; l < node["locations"].size(); l++)
            {
                const YAML::Node &location = node["locations"][l]["location"];
                servers[n].locations[l].active = 1;
                if (location["conf"] && !location["conf"].IsNull())
                    servers[n].locations[l].conf = location["conf"].AS_STR;

                if (location["path"] && !location["path"].IsNull())
                    servers[n].locations[l].path = location["path"].AS_STR;

                if (location["type"] && !location["type"].IsNull())
                    servers[n].locations[l].type = location["type"].AS_STR;

                if (location["val"] && !location["val"].IsNull())
                    servers[n].locations[l].val = location["val"].AS_STR;

                if (location["other"] && location["other"].size())
                {
                    const YAML::Node &other = location["other"];
                    for (int i = 0; i < other.size(); i++)
                        if (!other[i].IsNull())
                            servers[n].locations[l].other[i] = other[i].AS_STR;
                }
            }
        }
    }

    for (auto &server : servers)
    {
        if (server.active != 1)
            break;
        server.print();
    }
}