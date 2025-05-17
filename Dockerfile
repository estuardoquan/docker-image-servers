FROM alpine:latest AS camarero

USER root

COPY ./camarero /cpp

RUN apk update && \
    apk add --no-cache --virtual build-deps \
    build-base \
    abuild \
    cmake \ 
    extra-cmake-modules \
    git \
    wget && \
    cmake -B /cpp/build -S /cpp && \
    make --directory /cpp/build

FROM scratch AS server

COPY --from=nginx:alpine / /
COPY --from=camarero /cpp/build/camarero /usr/local/bin/camarero

COPY ./tmp/root_ca.crt /usr/local/share/ca-certificates/root_ca.crt
COPY ./certwatch.sh /usr/local/bin/certwatch.sh
COPY ./docker-entrypoint.sh /docker-entrypoint.sh

ADD ./etc/nginx/includes /etc/nginx/includes
ADD ./etc/nginx/yaml /etc/nginx/yaml
ADD ./etc/nginx/nginx.conf /etc/nginx/nginx.conf

RUN apk update && \
    apk add --no-cache \
    libgcc \
    libstdc++ \  
    inotify-tools && \
    chmod +x /docker-entrypoint.sh && \ 
    update-ca-certificates

ENTRYPOINT ["/docker-entrypoint.sh"]

CMD ["nginx", "-g", "daemon off;"]

