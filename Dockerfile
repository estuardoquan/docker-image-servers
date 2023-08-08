FROM nginx:alpine

USER root

# Install dependencies
RUN apk update
RUN apk add --no-cache \
    inotify-tools \
    jq

# Install executables
COPY ./bin /usr/local/bin

RUN chmod +x /usr/local/bin/certwatch.sh 
RUN chmod +x /usr/local/bin/ngconf.d/ngconf 

# RUN ln -s /src/ngcli /usr/local/bin/ngcli
# ENV STEPPATH=/var/local/step

CMD ["nginx", "-g", "daemon off;"]