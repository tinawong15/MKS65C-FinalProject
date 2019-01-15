#include "networking.h"
#include "game.h"
int total_players;
// void process(char *s);
// void subserver(int from_client);
void to_all_clients(struct client clients[], int num_players, char* msg);

static void sighandler(int signo){
  if (signo == SIGINT){
    printf("Closing pipes...\n");
    if(unlink("to_client") != -1){
      printf("Pipe to_client deleted.\n" );
    }
    if(unlink("to_parent") != -1){
      printf("Pipe to_parent deleted.\n" );
    }
    exit(0);
  }
}

int main() {

  int listen_socket;
  int joined = 0;
  int i;
  char buffer1[BUFFER_SIZE];
  listen_socket = server_setup();

  struct client player;
  struct client clients[MAX];
  char msg[BUFFER_SIZE];
  char total[2];
  while (total_players < MAX) {
    printf("Waiting for players to connect...\n");
    int client_socket = server_connect(listen_socket);

    strcpy(buffer1, "[server] Please type your name.");
    write(client_socket, buffer1, sizeof(buffer1));

    write(client_socket, "1", sizeof("1"));

    read(client_socket, buffer1, sizeof(buffer1));

    strcpy(player.name, buffer1);
    player.client_socket = client_socket;
    clients[total_players] = player;
    total_players++;

    strcpy(msg, player.name);
    strcat(msg, " has joined the game.");
    printf("%s\n", msg);

    if(total_players > 1) {
      strcpy(msg, "\n");
      sprintf(total, "%d", total_players);
      strcat(msg, total);
      strcat(msg, " players have joined the game. Continue? (y/n)?");
      joined = 0;

      for(i = 0; i < total_players; i++) {
        write(clients[i].client_socket, msg, sizeof(msg));
        write(clients[i].client_socket, "1", sizeof("1"));
        read(clients[i].client_socket, buffer1, sizeof(buffer1));

        if(!strcmp(buffer1, "y")) {
          joined++;
        }
        else {
          strcpy(msg, "Player ");
          strcat(msg, clients[i].name);
          strcat(msg, " has left.");
          to_all_clients(clients, total_players, msg);
          close(clients[i].client_socket);
        }
      }

      if (joined == total_players){
        break;
      }
    }
  }

  to_all_clients(clients, total_players, "\nWelcome to Checkers!\n");
  // write(clients[i].client_socket, "0", sizeof("0"));
  char board[65];
  strcpy(board, init_board());
  // printf("%s\n", board);
  for(i = 0; i < total_players; i++) {
    if(i % 2 == 0) {
      clients[i].team = 1;
    }
    else {
      clients[i].team = 2;
    }
  }

  // for (i = 0; i < player_num; i++){
  //   printf("%d\n", clients[i].team);
  // }
  //play_game(clients, total_players, board);
  char start[3];
  int is_ongoing = 1;
  char user_piece[4];
  char user_move[4];
  char selected_piece;
  int selected_piece_index;
  char buffer[4];
  int amount_of_moves = 0;
  int num_white = 12;
  int num_red = 12;
  int turn = 1;

  //int turn = 1; // 1 if it is white team's turn, 2 if it is red team's turn
  // printf("Do you want to start playing? [y/n]\n");
  // fgets(start, 3, stdin);
  // start[strlen(start)-1] = '\0';
  // if(strcmp(start, "y") == 0) {
    printf("Start Checkers Game: \n");
    display(board);

  while(is_ongoing) {
    if(turn == 1) {
      printf("White's turn\n");
      //Add King function
      // First checks if there are any jumps possible, starting from the bottom most part of the board.
      int x, i;
      printf("Starting search for jumps\n");
      for (i = 63; i >= 0 ;i--) {
        if (board[i] == 'x') {
          x = check_opponents(i, board);
          while (x == 3 || x == 4) {
            int new = jump('x', i, x, board);
            num_red --;
            display(board);
            x = check_opponents(new, board);
              //printf("%d\n",i);
            printf("found an enemy piece i'm White \n");
            break;
          }
        }
      }
      printf("Ended search for jumps\n");

      if (!x) {
        while(1) {
          printf("White checkers turn! There are no jumps available, so select a piece to move [row][column]: \n");
          write(clients[i].client_socket, "1", sizeof("1"));
          read(clients[i].client_socket, buffer, sizeof(buffer));
          strcpy(user_piece, buffer);
          //fgets(user_piece, 4, stdin);
          user_piece[strlen(user_piece)-1] = '\0';
          selected_piece = board[get_piece_position(user_piece, board)];
          if(selected_piece == 'x') {
            printf("Select where to move it [row][column]: \n");
            //fgets(user_move, 4, stdin);
            write(clients[i].client_socket, "1", sizeof("1"));
            read(clients[i].client_socket, buffer, sizeof(buffer));
            strcpy(user_move, buffer);
            user_move[strlen(user_move)-1] = '\0';
            if(is_viable_move(user_piece, user_move, board) == 1) {
            // moved diagonally
              board[get_piece_position(user_move, board)] = selected_piece;
              board[get_piece_position(user_piece, board)] = '-';
              break;
            }
            else {
              printf("You cannot move here. Try again.\n");
            }
          }
          else {
          printf("You cannot move this piece. Try again.\n");
          }
        }
      }
      /**
      if (x == 3 || x == 4 ) {

        printf("x : %d\n", x);
        printf("Ya jumping now.");
        int new = jump('x', i, x, board);
        printf("new position: %d\n", new);
        num_white --;
        display(board);
        x = check_opponents(new, board);
        printf("after checking opponents: %d\n", x);

        if (!x){
          printf("No more jumps available");
          turn = 1;
          break;
        }

        //break;

      }
      **/
      turn = 2;
    }

    else {
      printf("Red's turn.\n");
      // Add: King function
      // First checks if there are any jumps possible, starting from the upper most part of the board.
      int x, i;
      printf("Starting search for jumps\n");
      for (i = 0; i < 64 ;i++) {
        if (board[i] == 'o') {
          x = check_opponents(i, board);
          while (x == 1 || x == 2) {

            int new = jump('o', i, x, board);
            num_white --;
            display(board);
            x = check_opponents(new, board);
            //printf("%d\n",i);
            printf("found an enemy piece i'm red\n");
            break;
          }
        }
      }
      printf("Ended search for jumps\n");
      if (!x) {
        while(1) {
        printf("Red checkers turn! There are no jumps available, so select a piece to move [row][column]: \n");
        fgets(user_piece, 4, stdin);
        user_piece[strlen(user_piece)-1] = '\0';
        selected_piece = board[get_piece_position(user_piece, board)];
        if(selected_piece == 'o') {
          printf("Select where to move it [row][column]: \n");
          fgets(user_move, 4, stdin);
          user_move[strlen(user_move)-1] = '\0';
          if(is_viable_move(user_piece, user_move, board) == 1) {
            // moved diagonally
            board[get_piece_position(user_move, board)] = selected_piece;
            board[get_piece_position(user_piece, board)] = '-';
            break;
          }
          else {
            printf("You cannot move here. Try again.\n");
          }
        }
        else {
          printf("You cannot move this piece. Try again.\n");
          }
        }
      }
      /**
        if (x == 1 || x == 2) {
          printf("x : %d\n", x);
          printf("Ya jumping now.");
          int new = jump('o', i, x, board);
          printf("new position: %d\n", new);
          num_white --;
          display(board);
          x = check_opponents(new, board);
          printf("after checking opponents: %d\n", x);

          if (!x){
            printf("No more jumps available");
            turn = 1;
            break;
          }

          //break;
        }
        **/
        turn = 1;
      }
      amount_of_moves++;
      if(amount_of_moves == 50) {
        printf("50 moves have been reached. Game is a draw.\n");
        is_ongoing = 0;
      }
      if (num_white == 0) {
        printf("All white pieces have been taken. Red team wins. \n");
        is_ongoing = 0;
      }
      if (num_red == 0) {
        printf("All red pieces have been taken. White team wins. \n");
        is_ongoing = 0;
      }
      display(board);

    }
   // else {
   //   exit(0);
   // }

  return 0;
}

void to_all_clients(struct client clients[], int num_players, char* msg){
   int i;
   char buffer[BUFFER_SIZE];
   strcpy(buffer, msg);
   for (i = 0; i < num_players; i++){
     write(clients[i].client_socket, buffer, sizeof(buffer));
     write(clients[i].client_socket, "0", sizeof("0"));
   }
}

// void subserver(int client_socket) {
//   char buffer[BUFFER_SIZE];
//
//   while (read(client_socket, buffer, sizeof(buffer))) {
//
//     printf("[subserver %d] received: [%s]\n", getpid(), buffer);
//     process(buffer);
//     write(client_socket, buffer, sizeof(buffer));
//   }//end read loop
// }