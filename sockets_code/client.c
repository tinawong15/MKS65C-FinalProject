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
  printf("%s\n", user_board);

  while (1) {
    printf("Waiting for your turn...\n");
    while(read(server_socket, buffer, sizeof(buffer))) {
        //char * cards = buffer;
        //printf("hand: %s\n", buffer);
        display(user_board);
        //display buffer (either is new card or a hand
        if(strcmp(buffer, "lose") == 0) {
            //exit
        } else {
            printf("What do you want to do?\n");
            fgets(buffer, BUFFER_SIZE, stdin);
            if(strcmp(buffer, "display") == 0){
                display_cards(buffer);
            }
            else if(strcmp(buffer, "remove") == 0) {
                printf("What is the position of the card you want to remove? ");
                fgets(pos, POS_SIZE, stdin);
                *strchr(pos, '\n') = 0;
                //tell server which pos you want to discard
                write(server_socket, pos, sizeof(pos));
            }
            else if(strcmp(buffer, "s") == 0) {
                write(server_socket, "s", sizeof("s"));
            }
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
