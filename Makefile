CSRC=./src
CBUILD=${CSRC}/build
CFILE=/main

all:

cbuild:
	cmake -B $(CBUILD) -S $(CSRC)

crun: 
	$(COUT)$(CFILE) $(ARGS)

cleanup:
	rm -r $(CBUILD)

dbuild: nginx

service-nginx:
	docker build -t proxymurder/nginx:latest -f Dockerfile ./


	