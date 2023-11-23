#include <yaml-cpp/yaml.h>
#include "stdlib.h"
#include "server.h"
#include "server_yml.h"

#define STR as<std::string>()
#define INT as<int>()

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

        servers[n].port = node["port"].INT;

        servers[n].domain.subject = node["domain"]["subject"].STR;
        if (node["domain"]["alternate"])
            servers[n].domain.alternate = node["domain"]["alternate"].STR;

        if (node["index"] && !node["index"].IsNull())
            servers[n].index = node["index"].STR;

        if (node["root"] && !node["root"].IsNull())
            servers[n].root = node["root"].STR;

        if (node["log"] && !node["log"].IsNull())
            servers[n].log = node["log"].STR;

        if (node["https"] && !node["https"].IsNull())
        {
            servers[n].https.active = true;
            if (node["https"].IsMap())
            {
                if (node["https"]["path"] && !node["https"]["path"].IsNull())
                    servers[n].https.path = node["https"]["path"].STR;

                if (node["https"]["filename"] && !node["https"]["filename"].IsNull())
                    servers[n].https.filename = node["https"]["filename"].STR;

                if (node["https"]["include"] && !node["https"]["include"].IsNull())
                    servers[n].https.include = node["https"]["include"].STR;
            }
        }
        if (node["locations"] && node["locations"].size())
        {
            for (int l = 0; l < node["locations"].size(); l++)
            {
                if (node["locations"][l]["location"]["include"] &&
                    !node["locations"][l]["location"]["include"].IsNull())
                    servers[n].locations[l].include = node["locations"][l]["location"]["include"].STR;

                if (node["locations"][l]["location"]["path"] &&
                    !node["locations"][l]["location"]["path"].IsNull())
                    servers[n].locations[l].path = node["locations"][l]["location"]["path"].STR;

                if (node["locations"][l]["location"]["type"] &&
                    !node["locations"][l]["location"]["type"].IsNull())
                    servers[n].locations[l].type = node["locations"][l]["location"]["type"].STR;

                if (node["locations"][l]["location"]["value"] &&
                    !node["locations"][l]["location"]["value"].IsNull())
                    servers[n].locations[l].value = node["locations"][l]["location"]["value"].STR;

                if (node["locations"][l]["location"]["other"] &&
                    node["locations"][l]["location"]["other"].size())
                    for (int i = 0; i < node["locations"][l]["location"]["other"].size(); i++)
                        if (!node["locations"][l]["location"]["other"][i].IsNull())
                            servers[n].locations[l].other[i] = node["locations"][l]["location"]["other"][i].STR;

                servers[n].locations[l].active = 1;
            }
        }
        if (node["redirect"] && !node["redirect"].IsNull())
        {
            if (node["redirect"].IsScalar())
                servers[n].redirect.value = node["redirect"].INT;

            if (node["redirect"].IsMap())
            {
                if (node["redirect"]["value"] && !node["redirect"]["value"].IsNull())
                    servers[n].redirect.value = node["redirect"]["value"].INT;

                if (node["redirect"]["other"] && !node["redirect"]["other"].IsNull())
                    servers[n].redirect.other = node["redirect"]["other"].STR;
            }
            servers->redirect.active = true;
        }

        servers[n].print();
    }
}
