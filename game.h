#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include <math.h>
// #include <sys/ipc.h>
// #include <sys/sem.h>
// #include <sys/types.h>
// #include <sys/shm.h>
// #include <errno.h>
// #include <fcntl.h>
// #include <unistd.h>

void display(char * board);

int get_piece_position(char * input, char * board);

int is_viable_move(char * piece, char * move, char * board);

char * init_board();

void play(char * board);

char * help();
