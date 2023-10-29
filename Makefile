all:

build:
	cmake -B ./src/bin -S ./src

ngconf: ngconf-core ngconf-php

ngconf-core:
	docker build -t proxymurder/nginx:latest -f Dockerfile ./

ngconf-php:
	docker build -t proxymurder/nginx:php -f Dockerfile.php ./