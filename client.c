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

  while(1) {
    printf("[client] User input to server: ");
    fgets(msg, BUFFER_SIZE, stdin);
    msg[strlen(msg)-1] = '\0';
    printf("[client] Client sent: %s\n", msg);

    write(to_server, msg, BUFFER_SIZE);
    read(from_server, msg, BUFFER_SIZE);
    printf("%s\n", msg);
  }
  return 0;
}
