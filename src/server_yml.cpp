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
            if (node["https"].IsScalar())
                servers[n].https.path = node["https"].STR;

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
                if (node["locations"][l]["include"] &&
                    !node["locations"][l]["include"].IsNull())
                    servers[n].locations[l].include = node["locations"][l]["include"].STR;

                if (node["locations"][l]["path"] &&
                    !node["locations"][l]["path"].IsNull())
                    servers[n].locations[l].path = node["locations"][l]["path"].STR;

                if (node["locations"][l]["type"] &&
                    !node["locations"][l]["type"].IsNull())
                    servers[n].locations[l].type = node["locations"][l]["type"].STR;

                if (node["locations"][l]["value"] &&
                    !node["locations"][l]["value"].IsNull())
                    servers[n].locations[l].value = node["locations"][l]["value"].STR;

                if (node["locations"][l]["other"] &&
                    node["locations"][l]["other"].size())
                    for (int i = 0; i < node["locations"][l]["other"].size(); i++)
                        if (!node["locations"][l]["other"][i].IsNull())
                            servers[n].locations[l].other[i] = node["locations"][l]["other"][i].STR;

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
            servers[n].redirect.active = true;
        }

        servers[n].print();
    }
}
