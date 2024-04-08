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

COPY ./camarero /cpp
RUN chmod +x /cpp/certwatch

RUN cmake -B /cpp/build -S /cpp

RUN make --directory /cpp/build

FROM scratch AS servers

COPY --from=nginx:alpine / /

RUN apk update
RUN apk add --no-cache \
    libgcc \
    libstdc++ \  
    inotify-tools

COPY --from=build /cpp/build/camarero /usr/local/bin/camarero
COPY --from=build /cpp/certwatch /usr/local/bin/certwatch

COPY ./docker-entrypoint /docker-entrypoint

RUN chmod +x /docker-entrypoint 

ADD ./etc/nginx/includes /etc/nginx/includes
ADD ./etc/nginx/yaml /etc/nginx/yaml
ADD ./etc/nginx/nginx.conf /etc/nginx/nginx.conf

ENTRYPOINT ["/docker-entrypoint"]

CMD ["nginx", "-g", "daemon off;"]

