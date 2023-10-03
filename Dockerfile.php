FROM proxymurder/nginx:latest

COPY ./docker-entrypoint-php /docker-entrypoint
RUN chmod +x /docker-entrypoint 
