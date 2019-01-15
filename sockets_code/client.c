#include "networking.h"
#include "game.h"

int should_respond(int server_socket);

int should_respond(int server_socket){
   char buffer[BUFFER_SIZE];
   read(server_socket, buffer, sizeof(buffer));

   if(strcmp(buffer, "1") == 0){
     return 1;
   }
   else if(strcmp(buffer, "2") == 0){
     return 2;
   }
   else {
     return 0;
   }
}

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
          printf("It's your turn! Select a piece to move [row][column]: \n");
          printf("[client] ");
          fgets(buffer, sizeof(buffer), stdin);
          buffer[strlen(buffer)-1] = '\0';
          write(server_socket, buffer, sizeof(buffer));
        }
        else if (strcmp(buffer, "4") == 0) {
          printf("Select where to move it [row][column]: \n");
          printf("[client] ");
          fgets(buffer, sizeof(buffer), stdin);
          // printf("%s\n", buffer);
          buffer[strlen(buffer)-1] = '\0';
          write(server_socket, buffer, sizeof(buffer));
        }
        else if (strcmp(buffer, "10") == 0) {
          display(user_board);
          break;
        }
        else if(buffer[0] == '-') {
          strcpy(user_board, buffer);
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
/**
  while (read(server_socket, buffer, sizeof(buffer))) {
    strcpy(server_instructions, buffer);
    if(server_instructions[0] != '-' && server_instructions[0] != 'x' && server_instructions[0] != 'o') {
      printf("%s \n", server_instructions);
    }
    if (server_instructions[0] == '1' || server_instructions[0] == '2'){
      printf("what is going on after read?!: %s\n", server_instructions);
      team = (int)server_instructions[0];
    }
    if(team == 1) {
      // strcpy(user_board, server_instructions);
      display(user_board);
    }
    if(should_respond(server_socket)) {
      printf("[client] ");
      fgets(buffer, sizeof(buffer), stdin);
      buffer[strlen(buffer)-1] = '\0';
      if(ask == 0 && islower(buffer[0]) && (buffer[1] <= 8 && buffer[1] >= 1)) {
        strcpy(user_piece, buffer);
        ask++;
      }
      if(ask == 1 && islower(buffer[0]) && (buffer[1] <= 8 && buffer[1] >= 1)) {
        strcpy(user_move, buffer);
        ask = 0;
      }
      write(server_socket, buffer, sizeof(buffer));

    }

  // read(server_socket, buffer, sizeof(buffer));
  //   printf("received: [%s]\n", buffer);
  }
  **/
}
