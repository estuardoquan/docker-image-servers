FROM nginx:alpine AS ngconf-cli

USER root

RUN apk update

COPY ./bin/certwatch /usr/local/bin/certwatch
COPY ./bin/print_ngconf /usr/local/bin/print_ngconf

RUN chmod +x /usr/local/bin/certwatch /usr/local/bin/print_ngconf

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