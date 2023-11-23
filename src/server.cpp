#include <charsum.h>
#include <stdio.h>
#include <stdlib.h>
#include "server.h"

char *CLIServer::Domain::name()
{
    return charsum(&subject, &alternate);
}

void CLIServer::print()
{
    printf("server \n{\n");
    printf("\tlisten %s%s;\n", port, https.active ? " ssl" : "");

    name = domain.name();
    printf("\tserver_name %s;\n\n", name);
    free(name);

    if (log.active)
    {
        printf("\taccess_log %s/%s.access.log;\n", log.path, domain.subject);
        printf("\terror_log %s/%s.access.log;\n\n", log.path, domain.subject);
    }

    if (https.active)
    {
        printf("\tssl_certificate %s/%s.crt;\n", https.path, https.filename);
        printf("\tssl_certificate_key %s/%s.key;\n", https.path, https.filename);
        printf("\tinclude %s;\n\n", https.conf);
    }
    if (*root != '\0')
        printf("\troot %s;\n\n", root);
    if (*index != '\0')
        printf("\tindex %s;\n\n", index);
    for (auto &location : locations)
    {
        if (location.active == 0)
            break;
        printf("\tlocation %s\n\t{\n", location.path);
        if (*location.type != '\0')
            printf("\t\t%s %s;\n", location.type, location.val);
        if (*location.conf != '\0')
            printf("\t\tinclude %s;\n", location.conf);
        for (auto &o : location.other)
        {
            if (o == __null)
                break;
            printf("\t\t%s;\n", o);
        }
        printf("\t}\n\n");
    }
    printf("}\n");
}

void YMLServer::print()
{
    printf("server \n{\n");
    printf("\tlisten %s%s;\n", port.c_str(),
           https.active ? " ssl" : "");

    printf("\tserver_name %s%s;\n\n",
           domain.subject.c_str(),
           domain.alternate.empty() ? "" : (" " + domain.alternate).c_str());

    if (log.active)
    {
        printf("\taccess_log %s/%s.access.log;\n", log.path.c_str(), domain.subject.c_str());
        printf("\terror_log %s/%s.access.log;\n\n", log.path.c_str(), domain.subject.c_str());
    }

    if (https.active)
    {
        printf("\tssl_certificate %s/%s.crt;\n", https.path.c_str(), https.filename.c_str());
        printf("\tssl_certificate_key %s/%s.key;\n", https.path.c_str(), https.filename.c_str());
        printf("\tinclude %s;\n\n", https.conf.c_str());
    }
    if (!root.empty())
        printf("\troot %s;\n\n", root.c_str());
    if (!index.empty())
        printf("\tindex %s;\n\n", index.c_str());
    for (auto &location : locations)
    {
        if (location.active == 0)
            break;
        printf("\tlocation %s\n\t{\n", location.path.c_str());
        if (!location.type.empty())
            printf("\t\t%s %s;\n", location.type.c_str(), location.val.c_str());
        if (!location.conf.empty())
            printf("\t\tinclude %s;\n", location.conf.c_str());
        for (auto &o : location.other)
        {
            if (o.empty())
                break;
            printf("\t\t%s;\n", o.c_str());
        }
        printf("\t}\n\n");
    }
    printf("}\n");
}