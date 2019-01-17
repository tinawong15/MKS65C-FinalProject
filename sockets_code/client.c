#include "networking.h"
#include "game.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  int team;
  char server_instructions[BUFFER_SIZE];

  char user_board[BUFFER_SIZE];
  int ask = 0;
  char user_piece[4];
  char user_move[4];
  strcpy(user_board, init_board());
  // printf("%s\n", user_board);

  while (1) {
    //printf("Waiting for your turn...\n");
    while(read(server_socket, buffer, sizeof(buffer))) {
      // printf("received: [%s]\n", buffer);
        if(strcmp(buffer, "1") == 0) {
          printf("[client] ");
          fgets(buffer, sizeof(buffer), stdin);
          buffer[strlen(buffer)-1] = '\0';
          write(server_socket, buffer, sizeof(buffer));
        }
        else if (strcmp(buffer, "2") == 0) {
          printf("[server] Please type your name.\n");
          fgets(buffer, sizeof(buffer), stdin);
          buffer[strlen(buffer)-1] = '\0';
          write(server_socket, buffer, sizeof(buffer));
        }
        else if (strcmp(buffer, "3") == 0) {
          display(user_board);
          printf("It's your turn! Vote for a piece to move [row][column]: \n");
          printf("[client] ");
          fgets(buffer, sizeof(buffer), stdin);
          buffer[strlen(buffer)-1] = '\0';
          write(server_socket, buffer, sizeof(buffer));
        }
        else if (strcmp(buffer, "4") == 0) {
          printf("Vote where to move it [row][column]: \n");
          printf("[client] ");
          fgets(buffer, sizeof(buffer), stdin);
          // printf("%s\n", buffer);
          buffer[strlen(buffer)-1] = '\0';
          write(server_socket, buffer, sizeof(buffer));
        }
        else if (strcmp(buffer, "10") == 0) {
          display(user_board);
        }
        else if(buffer[0] == '-') {
          strcpy(user_board, buffer);
          break;
        }
        else if (strcmp(buffer, "exit") == 0) {
          exit(0);
        }
        else {
          printf("%s", buffer);
          break;
        }
      }
    }

}
