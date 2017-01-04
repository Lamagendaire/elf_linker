prog: readelf.o util.o
	gcc -o prog readelf.o util.o

readelf.o: readelf.c util.h
	gcc -c readelf.c 

util.o: util.c util.h
	gcc -c util.c
