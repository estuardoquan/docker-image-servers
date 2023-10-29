FROM nginx:alpine AS builder

USER root

RUN apk update
RUN apk add --no-cache \
    cmake

COPY ./src /src

RUN cmake -B /src/bin -S /src
RUN make --directory /src/bin

FROM nginx:alpine
# Install dependencies
RUN apk update
RUN apk add --no-cache \
    inotify-tools

COPY --from=builder /src/bin/printconf /usr/local/bin/printconf
# Install executables
COPY ./docker-entrypoint /docker-entrypoint
COPY ./certwatch /usr/local/bin/certwatch

RUN chmod +x /docker-entrypoint 
RUN chmod +x /usr/local/bin/certwatch

# ENV STEPPATH=/var/local/step

CMD ["nginx", "-g", "daemon off;"]

ENTRYPOINT ["/docker-entrypoint"]