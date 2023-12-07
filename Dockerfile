FROM alpine:latest AS build

USER root

RUN apk update
RUN apk add --no-cache --virtual build-deps\
    build-base \
    abuild \
    cmake \ 
    extra-cmake-modules \
    git \
    wget 

RUN mkdir -p /cpp

COPY ./src /cpp/src
COPY ./ext /cpp/ext
COPY ./CMakeLists.txt /cpp/CMakeLists.txt

RUN cmake -B /cpp/build -S /cpp

RUN make --directory /cpp/build

FROM scratch AS camarero

COPY --from=nginx:alpine / /

RUN apk update
RUN apk add --no-cache \
    libgcc \
    libstdc++ \  
    inotify-tools

COPY --from=build /cpp/build/camarero /usr/local/bin/camarero

COPY ./docker-entrypoint /docker-entrypoint
COPY ./certwatch /usr/local/bin/certwatch

RUN chmod +x /docker-entrypoint 
RUN chmod +x /usr/local/bin/certwatch

ADD ./etc/nginx/includes /etc/nginx/includes
ADD ./etc/nginx/yaml /etc/nginx/yaml
ADD ./etc/nginx/nginx.conf /etc/nginx/nginx.conf

ENTRYPOINT ["/docker-entrypoint"]

CMD ["nginx", "-g", "daemon off;"]