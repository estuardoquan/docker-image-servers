CDIR=./src
CBUILD=${CDIR}/build
CFILE=/main

all:

cbuild:
	cmake -B $(CBUILD) -S $(CDIR)

crun: 
	$(CBUILD)$(CFILE) $(ARGS)

cleanup:
	rm -rf $(CBUILD)

dbuild: service-nginx

service-nginx:
	docker build -t proxymurder/nginx:latest -f Dockerfile ./


	