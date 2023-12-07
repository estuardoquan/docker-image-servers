all: cleanup cbuild docker-all

cleanup:
	rm -rf ./camarero/build

cbuild:
	cmake -B ./camarero/build -S ./camarero

docker: docker-server-service

docker-all: docker-servers-build docker-servers-service

docker-servers-camarero:
	 docker build -t proxymurder/servers:build  --target build ./

docker-servers-service:
	docker build -t proxymurder/servers:latest  --target servers ./
