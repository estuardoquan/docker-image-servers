#include <stdio.h>
#include <stdlib.h>
#include "server.h"

void Server::print()
{
    printf("server\n{\n");
    printf("\tlisten %i%s;\n", port,
           https.active ? " ssl" : "");

    printf("\tserver_name %s%s;\n\n",
           domain.subject.c_str(),
           domain.alternate.empty() ? "" : (" " + domain.alternate).c_str());

    if (!log.empty())
    {
        printf("\taccess_log %s/%s.access.log;\n", log.c_str(), domain.subject.c_str());
        printf("\terror_log %s/%s.error.log;\n\n", log.c_str(), domain.subject.c_str());
    }

    if (https.active)
    {
        printf("\tssl_certificate %s/%s.crt;\n", https.path.c_str(), https.filename.c_str());
        printf("\tssl_certificate_key %s/%s.key;\n", https.path.c_str(), https.filename.c_str());
        printf("\tinclude %s;\n\n", https.include.c_str());
    }
    if (!root.empty())
        printf("\troot %s;\n\n", root.c_str());
    if (!index.empty())
        printf("\tindex %s;\n\n", index.c_str());

    for (auto &o : other)
    {
        if (o.empty())
            break;
        printf("\t%s;\n", o.c_str());
    }
    
    printf("\n");

    for (auto &location : locations)
    {
        if (location.active == 0)
            break;
        printf("\tlocation %s\n\t{\n", location.path.c_str());

        if (!location.include.empty())
            printf("\t\tinclude %s;\n", location.include.c_str());
        for (auto &o : location.other)
        {
            if (o.empty())
                break;
            printf("\t\t%s;\n", o.c_str());
        }
        if (!location.type.empty())
            printf("\t\t%s %s;\n", location.type.c_str(), location.value.c_str());
        printf("\t}\n\n");
    }
    printf("}\n");
    if (redirect.active)
    {
        printf("server\n{\n");
        printf("\tlisten %i;\n", redirect.value);
        printf("\tserver_name %s%s;\n\n",
               domain.subject.c_str(),
               domain.alternate.empty() ? "" : (" " + domain.alternate).c_str());
        printf("\treturn 301 %s;\n\n", redirect.other.c_str());
        printf("}\n\n");
    }
}
