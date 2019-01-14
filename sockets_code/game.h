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

#define BUFFER_SIZE 1024
#define MAX 2

struct client {
  int client_socket;
  char name[256];
  int team;
};

void display(char * board);

int get_piece_position(char * input, char * board);

int is_viable_move(char * piece, char * move, char * board);

char * init_board();

void play(char * board);

char * client_game(char * board);

int play_game(struct client clients[], int num_players, char * board);
