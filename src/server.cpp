#include <charsum.h>
#include <stdio.h>
#include <stdlib.h>
#include "server.h"

char *Domain::name()
{
    return charsum(&subject, &alternate);
}

void Server::print()
{
    printf("server \n{\n");
    printf("\tlisten %s%s;\n", port, https.active ? " ssl" : "");

    name = domain.name();
    printf("\tserver_name %s;\n\n", name);
    free(name);

    if (log.active)
    {
        printf("\taccess_log %s/%s.access.log;\n", log.path, *log.filename);
        printf("\terror_log %s/%s.access.log;\n\n", log.path, *log.filename);
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
        // for (auto &o : location.other)
        // {
        //     if (o == __null)
        //         break;
        //     printf("\t\t&o:%s\n", o);
        // }
        printf("\t}\n\n");
    }
    printf("}\n");
}