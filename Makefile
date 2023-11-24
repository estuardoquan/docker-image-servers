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

docker: docker-server

docker-server:
	docker build -t proxymurder/server:latest  ./

docker-nserver:
	# docker build -t proxymurder/server:latest  ./
