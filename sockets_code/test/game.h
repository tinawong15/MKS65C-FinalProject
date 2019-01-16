#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MAX 2

struct client {
  int client_socket;
  char name[256];
  int team;
  int selected;
};

void display(char * board);

int get_piece_position(char * input, char * board);

int is_viable_move(char * piece, char * move, char * board);

char * init_board();

int check_opponents(int piece_loc, char * board);

int jump(char piece, int piece_loc, int direction, char * board);
