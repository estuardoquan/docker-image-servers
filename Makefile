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

docker: camarero-build camarero

camarero-build:
	 docker build -t proxymurder/camarero:build  --target build ./

camarero:
	docker build -t proxymurder/camarero:latest  --target camarero ./
