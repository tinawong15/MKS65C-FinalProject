#include "networking.h"
#include "game.h"
int should_respond(int server_socket);

int should_respond(int server_socket){
   char buffer[BUFFER_SIZE];
   read(server_socket, buffer, sizeof(buffer));

   if(strcmp(buffer, "1") == 0){
     return 1;
   }
   else {
     return 0;
   }
}

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];
  char user_board[BUFFER_SIZE];
  strcpy(user_board, init_board());
  printf("%s\n", user_board);
  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  int i = 0;
  while (read(server_socket, buffer, sizeof(buffer))) {
    if(buffer[0] != '-') {
      printf("%s \n", buffer);
      i++;
    }
    else {
      // strcpy(user_board, buffer);
      display(user_board);
    }
    if(should_respond(server_socket)) {
      printf("[client] ");
      fgets(buffer, sizeof(buffer), stdin);
      buffer[strlen(buffer)-1] = '\0';
      write(server_socket, buffer, sizeof(buffer));

    }

  // read(server_socket, buffer, sizeof(buffer));
  //   printf("received: [%s]\n", buffer);
  }
}
