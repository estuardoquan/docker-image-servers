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

docker: camarerc camarero

camarerc:
	 docker build -t proxymurder/camarero:builder  --target builder ./

camarero:
	docker build -t proxymurder/camarero:latest  --target camarero ./
