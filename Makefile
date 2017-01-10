prog: main.o util.o display.o readelf.o fusionelf.o
	gcc -o prog main.o util.o display.o readelf.o fusionelf.o $(CFlags)

main.o: main.c util.h
	gcc -c main.c $(CFlags)

display.o: display.c display.h readelf.h
	gcc -c display.c $(CFlags)
	
util.o: util.c util.h
	gcc -c util.c $(CFlags)

readelf.o: readelf.c readelf.h
	gcc -c readelf.c $(CFlags)

fusionelf.o: fusionelf.c fusionelf.h
	gcc -c fusionelf.c $(CFlags)

CFlags=-Wall -g
