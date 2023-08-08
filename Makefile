all:

nginx-core:
	docker build -t proxymurder/nginx:latest ./Dockerfile
nginx-php:
	docker build -t proxymurder/nginx:php ./Dockerfile.php