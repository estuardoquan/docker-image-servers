all:

out:
	cmake -B ./src/out -S ./src

run: 
	./src/out/server $(options)

ngconf: ngconf-core ngconf-php

ngconf-core:
	docker build -t proxymurder/nginx:latest -f Dockerfile ./

ngconf-php:
	docker build -t proxymurder/nginx:php -f Dockerfile.php ./