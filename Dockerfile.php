FROM proxymurder/nginx:latest

COPY ./docker-entrypoint /docker-entrypoint
RUN chmod +x /docker-entrypoint 

ENTRYPOINT ["/docker-entrypoint"]