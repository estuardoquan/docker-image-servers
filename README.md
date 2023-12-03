### The following backend configuration can be created through:

```
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

CLI command

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

Alt to the CLI command a yaml conf may also be used to create this same structure.

Yaml has to be loaded through CLI

```
camarero --yaml /path/to/conf.yaml
```

```
# conf.yaml // configuration for php running on a docker container named php on port 9000
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

Finally, this is the SET of ENV variables for docker-compose.

```
# docker-entrypoint CLI approach, can only make one server
SERVER_PORT: 6000
SERVER_NAME: localhost
SERVER_SAN: "\*.localhost"
SERVER_HTTPS: /var/local/step
SERVER_LOG: /var/log/nginx
SERVER_ROOT: /var/www/html/public
SERVER_INDEX: index.html index.php
SERVER_TRYFILES: $$uri $$uri/ /index.php?$$query_string
SERVER_PHP: php:9000

# For YAML file you may declare de following variables to both,
# load the conf.yaml file and keep watches alive.
# loads multiple servers
SERVER_YAML
SERVER_WATCH_CRT
```
