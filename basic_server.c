#include "pipe_networking.h"

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

    if(from_client) {
      while(read(from_client, msg, BUFFER_SIZE)) {
        printf("Server received: %s\n", msg);
        for(i = 0; i < strlen(msg); i++) {
          msg[i] += 1;
        }
        write(to_client, msg, BUFFER_SIZE);
        printf("Server sent: %s\n", msg);
      }
      printf("Client disconnected. Time for a new handshake!\n");
    }
  }
  return 0;
}
