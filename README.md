### The following backend configuration can be created through:

This set of ENV variables in docker with proxymurder/camarero:latest

```
SERVER_PORT: 6000
SERVER_NAME: localhost
SERVER_SAN: "\*.localhost"
SERVER_HTTPS: /var/local/step
SERVER_LOG: /var/log/nginx
SERVER_ROOT: /var/www/html/public
SERVER_INDEX: index.html index.php
SERVER_TRYFILES: $$uri $$uri/ /index.php?$$query_string
SERVER_PHP: php:9000

# For YAML file you may declare de following variables;
# to load the conf.yaml file and keep watches alive.
SERVER_YAML
SERVER_WATCH_CRT
```

Generated this .conf file for php running on a docker container on port 9000.

```
# /etc/nginx/conf.d/default.conf

server {
    listen 6000 ssl;
    server_name localhost *.localhost;

    access_log /var/log/nginx/localhost.access.log;
    error_log /var/log/nginx/localhost.access.log;

    ssl_certificate /var/local/step/site.crt;
    ssl_certificate_key /var/local/step/site.key;
    include /etc/nginx/includes/default.https.conf;

    root /var/www/html/public;

    index index.html index.php;

    location / {
        try_files $uri $uri/ /index.php?$query_string;
    }

    location ~ \.php$ {
        fastcgi_pass php:9000;
        include /etc/nginx/includes/default.php_fastcgi.conf;
    }

    location /favicon.ico {
        access_log off;
        log_not_found off;
    }

    location /robots.txt {
        access_log off;
        log_not_found off;
    }

    location ~ /\.ht {
        deny all;
    }

}
```

Docker compose entrypoint generates the following, CLI command:

```
 camarero print --port 6000 --name localhost --san *.localhost                                                                             \
           --https path="/var/local/step",include="/etc/nginx/includes/default.https.conf",filename="site"                                 \
           --log /var/log/nginx --root /var/www/html/public --index "index.html index.php"                                                 \
           --location path="/",type="try_files",value="\$uri \$uri/ /index.php?$query_string",include=,other="gzip_static on"              \
           --location path="~ \.php$",type="fastcgi_pass",value="php:9000",include="/etc/nginx/includes/default.php_fastcgi.conf"          \
           --location path="/favicon.ico",type="access_log",value="off",other="log_not_found off"                                          \
           --location path="/robots.txt",type="access_log",value="off",other="log_not_found off"                                           \
           --location path="~ /\.ht",type="deny",value="all"                                                                               \
```

Alternative to the CLI command a YAML conf may also be used to create this same structure.

Passed through ENV variable, and loaded through:

```
camarero --yaml /path/to/conf.yaml
```

```
# conf.yaml
 - server:
       port: 6000
       domain:
           subject: localhost
           alternate: '*.localhost'                            # '*' due to yaml anchors
       https: true
       log: true
       root: /var/www/html/public
       index: index.html index.php
       locations:
           - value: $uri $uri/ /index.php?$query_string
             other:
                 - 'gzip_static on'
           - path: ~ \.php
             type: fastcgi_pass
             value: php:9000
             include: /etc/nginx/includes/default.php_fastcgi.conf
           - path: /favicon.ico
             other:
                 - 'access_log off'
                 - 'log_not_found off'
           - path: /robots.txt
             other:
                 - 'access_log off'
                 - 'log_not_found off'
           - path: ~ /\.ht
             type: deny
             value: all
```

Finally this is the complete list of available variables and their initianized values:

```
# docker-entrypoint

SERVER_PORT=${SERVER_PORT:-5000}
SERVER_NAME=${SERVER_NAME:-localhost}
SERVER_SAN=${SERVER_SAN:-""}

SERVER_HTTPS=${SERVER_HTTPS:-""}
SERVER_HTTPS_INCLUDE=${SERVER_HTTPS_INCLUDE:-/etc/nginx/includes/default.https.conf}
SERVER_HTTPS_FNAME=${SERVER_HTTPS_FNAME:-site}

SERVER_WATCH_CRT=${SERVER_WATCH_CRT:-""}

SERVER_LOG=${SERVER_LOG:-""}
SERVER_ROOT=${SERVER_ROOT:-""}
SERVER_INDEX=${SERVER_INDEX:-""}

SERVER_LOCATION=${SERVER_LOCATION:-""}

SERVER_TRYFILES=${SERVER_TRYFILES:-""}
SERVER_TRYFILES_INCLUDE=${SERVER_TRYFILES_INCLUDE:-""}
SERVER_TRYFILES_PATH=${SERVER_TRYFILES_PATH:-"/"}
SERVER_TRYFILES_OTHER=${SERVER_TRYFILES_OTHER:-""}

SERVER_PHP=${SERVER_PHP:-""}
SERVER_PHP_INCLUDE=${SERVER_PHP_INCLUDE:-/etc/nginx/includes/default.php_fastcgi.conf}

SERVER_FAVICON=${SERVER_FAVICON:-0}
SERVER_ROBOTS=${SERVER_ROBOTS:-0}
SERVER_HT=${SERVER_HT:-0}

SERVER_REDIRECT=${SERVER_REDIRECT:-""}

SERVER_YAML=${SERVER_YAML:-""}
```
