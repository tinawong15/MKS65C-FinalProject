all: game2.o
	gcc game2.o

game2.o: game2.c game.h
	gcc -c game2.c

run:
	./a.out

clean:
	rm -rf *.o a.out
