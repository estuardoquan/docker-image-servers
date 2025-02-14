AUTHOR=dqio
IMAGE=servers

TAG=${AUTHOR}/${IMAGE}

all: cleanup cbuild docker-build

cleanup:
	rm -rf ./camarero/build

cbuild:
	cmake -B ./camarero/build -S ./camarero

docker-build: docker-build-camarero docker-build-server

docker-build-camarero:
	docker build -t ${TAG}:build  --target camarero ./

docker-build-server:
	docker build -t ${TAG}:latest  --target server ./
