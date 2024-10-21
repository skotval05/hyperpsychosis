CC = gcc
CFLAGS= -I.

client: client.o
	$(CC) -o client client.o

server: server.o
	$(CC) -o server server.o

client.o: client.c
	$(CC) -c client.c $(CFLAGS)

server.o: server.c
	$(CC) -c server.c $(CFLAGS)
