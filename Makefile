all: server client

clean:
	rm server client

server: server.c
	gcc -o server server.c

client: client.c
	gcc -o client client.c
