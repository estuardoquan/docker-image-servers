FROM nginx:alpine AS ngconf-cli

USER root

RUN apk update

COPY ./bin /usr/local/bin

RUN chmod +x /usr/local/bin/certwatch 
RUN chmod +x /usr/local/bin/ngconf.d/ngconf 
RUN ln -s /usr/local/bin/ngconf.d/ngconf /usr/local/bin/ngconf

FROM ngconf-cli AS nginx-server
# Install dependencies
RUN apk update
RUN apk add --no-cache \
    inotify-tools

# Install executables
COPY ./docker-entrypoint /docker-entrypoint
RUN chmod +x /docker-entrypoint 

# ENV STEPPATH=/var/local/step

CMD ["nginx", "-g", "daemon off;"]

ENTRYPOINT ["/docker-entrypoint"]