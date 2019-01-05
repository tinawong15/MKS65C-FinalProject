all: game.o
	gcc game.o

game.o: game.c game.h
	gcc -c game.c

run:
	./a.out

clean:
	rm -rf *.o a.out
