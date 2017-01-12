prog: display.o main.o util.o  readelf.o fusionelf.o
	$(CC) -o prog main.o util.o display.o readelf.o fusionelf.o $(CFlags)

main.o: main.c util.h
	$(CC) -c main.c $(CFlags)

display.o: display.c display.h readelf.h
	$(CC) -c display.c $(CFlags)
	
util.o: util.c util.h
	$(CC) -c util.c $(CFlags)

readelf.o: readelf.c readelf.h
	$(CC) -c readelf.c $(CFlags)

fusionelf.o: fusionelf.c fusionelf.h
	$(CC) -c fusionelf.c $(CFlags)

clean:
	rm prog main.o display.o util.o readelf.o fusionelf.o

CFlags=-Wall -g
CC=gcc -std=c99