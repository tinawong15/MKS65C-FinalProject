#include "pipe_networking.h"
# include "game.h"

static void sighandler(int signo){
  if(signo == SIGINT){
    printf("Removing well known pipe...\n");
    remove("server");
    printf("Server is exiting...\n");
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);

  int to_client;
  int from_client;
  char msg[BUFFER_SIZE];
  int i;

  while(1) {
    from_client = server_handshake( &to_client );
    write(to_client, "[server] Welcome to the Checkers game! Do you want to start playing? [y/n]", BUFFER_SIZE);
    read(from_client, msg, BUFFER_SIZE);
    char * board = init_board();
    if(from_client) {
      while(read(from_client, msg, BUFFER_SIZE)) {
        printf("[server] Server received: %s\n", msg);
        for(i = 0; i < strlen(msg); i++) {
          msg[i] += 1;
        }
        write(to_client, msg, BUFFER_SIZE);
        printf("[server] Server sent: %s\n", msg);
      }
      printf("[server] Client disconnected. Time for a new handshake!\n");
    }
  }
  return 0;
}
