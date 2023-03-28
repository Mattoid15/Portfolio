CC = gcc

all: client server

client.o: client.c
	$(CC) -c client.c

libraries.o: libraries.c libraries.h
	$(CC) -c libraries.c

client: client.o libraries.o
	$(CC) -o client client.o libraries.o

server.o: server.c
	$(CC) -c server.c

server: server.o libraries.o
	$(CC) -o server server.o libraries.o

clean:
	rm -f client server *.o