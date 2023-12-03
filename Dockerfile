FROM alpine:latest AS builder

USER root

RUN apk update
RUN apk add --no-cache --virtual build-deps\
    build-base \
    abuild \
    cmake \ 
    extra-cmake-modules \
    git \
    wget 

COPY ./src /src

RUN cmake -B /src/build -S /src
RUN make --directory /src/build

FROM nginx:alpine AS camarero

RUN apk update
RUN apk add --no-cache \
    libgcc \
    libstdc++ \  
    inotify-tools

ADD ./nginx/includes /etc/nginx/includes
ADD ./nginx/yaml /etc/nginx/yaml
ADD ./nginx/nginx.conf /etc/nginx/nginx.conf

COPY ./docker-entrypoint /docker-entrypoint
COPY ./certwatch /usr/local/bin/certwatch

RUN chmod +x /docker-entrypoint 
RUN chmod +x /usr/local/bin/certwatch

COPY --from=builder /src/build/camarero /usr/local/bin/camarero

CMD ["nginx", "-g", "daemon off;"]

ENTRYPOINT ["/docker-entrypoint"]