#include "pipe_networking.h"
# include "game.h"

static void sighandler(int signo){
  if(signo == SIGINT){
    printf("Client is exiting...\n");
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);

  int to_server;
  int from_server;
  char msg[BUFFER_SIZE];

  from_server = client_handshake( &to_server );

  printf("[client] ");
  fgets(msg, BUFFER_SIZE, stdin);
  msg[strlen(msg)-1] = '\0';

  write(to_server, msg, BUFFER_SIZE);
  printf("%s\n", "Welcome to the Checkers game!");
  while(1) {
    read(from_server, msg, BUFFER_SIZE);
    char * user_board = msg;
    // printf("%s\n", user_board);
    display(user_board);
    play(user_board);
  }
  return 0;
}
