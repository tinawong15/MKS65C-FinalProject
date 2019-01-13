all: client server

client: client.o pipe_networking.o game2.o
	gcc -o client client.o pipe_networking.o game2.o

server: basic_server.o pipe_networking.o game2.o
	gcc -o server basic_server.o pipe_networking.o game2.o

client.o: client.c pipe_networking.h game.h
	gcc -c client.c

basic_server.o: basic_server.c pipe_networking.h
	gcc -c basic_server.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	gcc -c pipe_networking.c

game2.o: game2.c game.h
		gcc -c game2.c

run:
	./a.out

clean:
	rm -rf *.o a.out
