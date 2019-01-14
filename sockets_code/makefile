forking: client fserver

fserver: forking_server.o networking.o game2.o
	gcc -o server forking_server.o networking.o game2.o

client: client.o networking.o game2.o
	gcc -o client client.o networking.o game2.o

client.o: client.c networking.h
	gcc -c client.c

forking_server.o: forking_server.c networking.h
	gcc -c forking_server.c

game2.o: game2.c game.h
	gcc -c game2.c

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm *.o
	rm *~
