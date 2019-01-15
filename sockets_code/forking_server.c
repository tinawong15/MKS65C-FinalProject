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
    //write(client_socket, buffer1, sizeof(buffer1));
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
        printf("%s\n", msg);
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
  // ====================================================================
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
  // ====================================================================

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
  char buffer2[BUFFER_SIZE];
  char buffer3[BUFFER_SIZE];
  char buffer4[BUFFER_SIZE];
  int amount_of_moves = 0;
  int num_white = 12;
  int num_red = 12;
  int turn = 1;

  //int turn = 1; // 1 if it is white team's turn, 2 if it is red team's turn
  // printf("Do you want to start playing? [y/n]\n");
  // fgets(start, 3, stdin);
  // start[strlen(start)-1] = '\0';
  // if(strcmp(start, "y") == 0) {
  to_all_clients(clients, total_players, "\nThe Checkers game is starting.\n");
  display(board);

  while(is_ongoing) {
    if(turn == 1) {
      to_all_clients(clients, total_players, "White's turn.\n");
      //Add King function
      // First checks if there are any jumps possible, starting from the bottom most part of the board.
      // printf("%s\n", "not jumping?");
      // display(board);
      int x, a;
      printf("Starting search for jumps\n");
      for (a = 63; a >= 0 ;a--) {
        if (board[a] == 'x') {
          printf("looking at %d\n", a);
          x = check_opponents(i, board);
          printf("looking for opponents");
          printf("%d\n", x);
          if (x == 3 || x == 4) {
            int new = jump('x', a, x, board);
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
      //write(clients[i].client_socket, "1", sizeof("1"));
      if (!x) {
        while(1) {
          //printf("White checkers turn! There are no jumps available, so select a piece to move [row][column]: \n");
          //write(clients[1].client_socket, "10", sizeof("10"));
          //strcpy(msg, "White checkers turn! There are no jumps available, so select a piece to move [row][column]: \n");
          //printf("%s\n", msg);
          write(clients[1].client_socket, "3", sizeof("3"));
          read(clients[1].client_socket, buffer2, sizeof(buffer2));
          printf(" user input:%s\n", buffer2);
          strcpy(user_piece, buffer2);
          //fgets(user_piece, 4, stdin);
          selected_piece = board[get_piece_position(user_piece, board)];
          printf("%c\n", selected_piece);
          if(selected_piece == 'x') {
            printf("Select where to move it [row][column]: \n");
            //fgets(user_move, 4, stdin);
            write(clients[1].client_socket, "4", sizeof("4"));
            printf("start reading\n");
            read(clients[1].client_socket, buffer2, sizeof(buffer2));
            printf("done reading\n");
            printf(" user input:%s\n", buffer2);
            strcpy(user_move, buffer2);
            printf("user input:%s\n", user_move);
            if(is_viable_move(user_piece, user_move, board) == 1) {
            // moved diagonally
              if (get_piece_position(user_move, board) >56 )
                board[get_piece_position(user_move, board)] = 'X';
              board[get_piece_position(user_move, board)] = selected_piece;
              board[get_piece_position(user_piece, board)] = '-';\

              break;
            }
            else if(is_viable_move(user_piece, user_move, board) == 2) {
            // then the piece jumped right
              selected_piece_index = get_piece_position(user_piece, board);
              board[selected_piece_index+7] = '-';
              if (get_piece_position(user_move, board) >56 )
                board[get_piece_position(user_move, board)] = 'X';
              else
                board[get_piece_position(user_move, board)] = selected_piece;

              board[get_piece_position(user_piece, board)] = '-';
              num_red--;
              break;
            }
            else if(is_viable_move(user_piece, user_move, board) == 3) {
              // then the piece jumped left
              selected_piece_index = get_piece_position(user_piece, board);
              board[selected_piece_index+9] = '-';
              if (get_piece_position(user_move, board) > 55 )
                board[get_piece_position(user_move, board)] = 'X';
              else
                board[get_piece_position(user_move, board)] = selected_piece;
              board[get_piece_position(user_piece, board)] = '-';
              num_red--;
              break;
            }
            else {
              strcpy(buffer4, "You cannot move here. Try again.\n");
              // printf("%s\n", buffer4);
              write(clients[1].client_socket, buffer4, sizeof(buffer4));
            }

          }
          else if(selected_piece == 'X') {
            printf("Select where to move it [row][column]: \n");
            //fgets(user_move, 4, stdin);
            write(clients[1].client_socket, "4", sizeof("4"));
            printf("start reading\n");
            read(clients[1].client_socket, buffer2, sizeof(buffer2));
            printf("done reading\n");
            printf(" user input:%s\n", buffer2);
            strcpy(user_move, buffer2);
            printf("user input:%s\n", user_move);
            if(is_viable_move(user_piece, user_move, board) == 1) {
            // moved diagonally
              board[get_piece_position(user_move, board)] = selected_piece;
              board[get_piece_position(user_piece, board)] = '-';\

              break;
            }
            else if(is_viable_move(user_piece, user_move, board) == 2) {
            // then the piece jumped right
              selected_piece_index = get_piece_position(user_piece, board);
              board[selected_piece_index+7] = '-';
              board[get_piece_position(user_move, board)] = selected_piece;

              board[get_piece_position(user_piece, board)] = '-';
              num_red--;
              break;
            }
            else if(is_viable_move(user_piece, user_move, board) == 3) {
              // then the piece jumped left
              selected_piece_index = get_piece_position(user_piece, board);
              board[selected_piece_index+9] = '-';
              board[get_piece_position(user_move, board)] = selected_piece;
              board[get_piece_position(user_piece, board)] = '-';
              num_red--;
              break;
            }
            else if(is_viable_move(user_piece, user_move, board) == 4) {
              // then the piece jumped backwards
              selected_piece_index = get_piece_position(user_piece, board);
              board[selected_piece_index-8] = '-';

              board[get_piece_position(user_move, board)] = selected_piece;
              board[get_piece_position(user_piece, board)] = '-';
              num_red--;
              break;
            }
            else if(is_viable_move(user_piece, user_move, board) == 5) {
              // then the piece jumped backwards
              board[get_piece_position(user_move, board)] = selected_piece;
              board[get_piece_position(user_piece, board)] = '-';\

              break;
            }
            else {
              strcpy(buffer4, "You cannot move here. Try again.\n");
              // printf("%s\n", buffer4);
              write(clients[1].client_socket, buffer4, sizeof(buffer4));
            }

          }
          else {
            strcpy(buffer4, "You cannot move this piece. Try again.\n");
            write(clients[1].client_socket, buffer4, sizeof(buffer4));
          }

        }
        display(board);
        turn = 2;
        strcpy(buffer3, board);
        write(clients[0].client_socket, buffer3, sizeof(buffer3));
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
    }

    else {
      to_all_clients(clients, total_players, "Red's turn.\n");
      // printf("%s\n", "not jumping?");
      // display(board);
      // Add: King function
      // First checks if there are any jumps possible, starting from the upper most part of the board.
      int x, b;
      printf("Starting search for jumps\n");
      for (b = 0; b < 64 ;b++) {
        if (board[b] == 'o') {
          x = check_opponents(i, board);
          printf("looking for opponents");
          printf("%d\n", x);

          if (x == 1 || x == 2) {

            int new = jump('o', b, x, board);
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
          write(clients[0].client_socket, "3", sizeof("3"));
          read(clients[0].client_socket, buffer2, sizeof(buffer2));
          printf(" user input:%s\n", buffer2);
          strcpy(user_piece, buffer2);
          //printf("Red checkers turn! There are no jumps available, so select a piece to move [row][column]: \n");
          //fgets(user_piece, 4, stdin);
          //user_piece[strlen(user_piece)-1] = '\0';

          selected_piece = board[get_piece_position(user_piece, board)];
          printf("%c\n", selected_piece);

          if(selected_piece == 'o') {
            //printf("Select where to move it [row][column]: \n");
            //fgets(user_move, 4, stdin);
            //user_move[strlen(user_move)-1] = '\0';
            write(clients[0].client_socket, "4", sizeof("4"));
            printf("start reading\n");
            read(clients[0].client_socket, buffer2, sizeof(buffer2));            printf("done reading\n");
            printf(" user input:%s\n", buffer2);
            strcpy(user_move, buffer2);
            printf("user input:%s\n", user_move);
            if(is_viable_move(user_piece, user_move, board) == 1) {
              // moved diagonally
              if (get_piece_position(user_move, board) < 8 )
                board[get_piece_position(user_move, board)] = 'O';
              board[get_piece_position(user_move, board)] = selected_piece;
              board[get_piece_position(user_piece, board)] = '-';
              break;
            }
            else if(is_viable_move(user_piece, user_move, board) == 2) {
            // then the piece jumped right
            selected_piece_index = get_piece_position(user_piece, board);
            board[selected_piece_index-7] = '-';
            if (get_piece_position(user_move, board) < 8 )
              board[get_piece_position(user_move, board)] = 'O';
            else
              board[get_piece_position(user_move, board)] = selected_piece;
            board[get_piece_position(user_piece, board)] = '-';
            num_white--;
            break;
            }
            else if(is_viable_move(user_piece, user_move, board) == 3) {
              // then the piece jumped left
              selected_piece_index = get_piece_position(user_piece, board);
              board[selected_piece_index-9] = '-';
              if (get_piece_position(user_move, board) < 8 )
                board[get_piece_position(user_move, board)] = 'O';
              else
                board[get_piece_position(user_move, board)] = selected_piece;
              board[get_piece_position(user_piece, board)] = '-';
              num_white--;
              break;
            }
            else {
              strcpy(buffer4, "You cannot move here. Try again.\n");
              write(clients[0].client_socket, buffer4, sizeof(buffer4));
            }
          }
          else if(selected_piece == 'O') {
            //printf("Select where to move it [row][column]: \n");
            //fgets(user_move, 4, stdin);
            //user_move[strlen(user_move)-1] = '\0';
            write(clients[0].client_socket, "4", sizeof("4"));
            printf("start reading\n");
            read(clients[0].client_socket, buffer2, sizeof(buffer2));            printf("done reading\n");
            printf(" user input:%s\n", buffer2);
            strcpy(user_move, buffer2);
            printf("user input:%s\n", user_move);
            if(is_viable_move(user_piece, user_move, board) == 1) {
              // moved diagonally
              board[get_piece_position(user_move, board)] = selected_piece;
              board[get_piece_position(user_piece, board)] = '-';
              break;
            }
            else if(is_viable_move(user_piece, user_move, board) == 2) {
            // then the piece jumped right
            selected_piece_index = get_piece_position(user_piece, board);
            board[selected_piece_index-7] = '-';

            board[get_piece_position(user_move, board)] = selected_piece;
            board[get_piece_position(user_piece, board)] = '-';
            num_white--;
            break;
            }
            else if(is_viable_move(user_piece, user_move, board) == 3) {
              // then the piece jumped left
              selected_piece_index = get_piece_position(user_piece, board);
              board[selected_piece_index-9] = '-';
              board[get_piece_position(user_move, board)] = selected_piece;
              board[get_piece_position(user_piece, board)] = '-';
              num_white--;
              break;
            }
            else if(is_viable_move(user_piece, user_move, board) == 4) {
              // then the piece jumped left
              selected_piece_index = get_piece_position(user_piece, board);
              board[selected_piece_index+8] = '-';
              board[get_piece_position(user_move, board)] = selected_piece;
              board[get_piece_position(user_piece, board)] = '-';
              num_white--;
              break;
            }
            else if(is_viable_move(user_piece, user_move, board) == 5) {
              // then the piece jumped backwards
              board[get_piece_position(user_move, board)] = selected_piece;
              board[get_piece_position(user_piece, board)] = '-';\

              break;
            }
            else {
              strcpy(buffer4, "You cannot move here. Try again.\n");
              write(clients[0].client_socket, buffer4, sizeof(buffer4));
            }
          }
          else {
            strcpy(buffer4, "You cannot move this piece. Try again.\n");
            write(clients[0].client_socket, buffer4, sizeof(buffer4));
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
        display(board);
        turn = 1;
        strcpy(buffer3, board);
        display(buffer3);
        write(clients[1].client_socket, buffer3, sizeof(buffer3));

    }
      amount_of_moves++;
      if(amount_of_moves == 50) {
        //strcpy(msg, "50 moves have been reached. Game is a draw.\n");
        to_all_clients(clients, total_players, "50 moves have been reached. Game is a draw.\n");

        is_ongoing = 0;
      }
      if (num_white == 0) {
        printf("All white pieces have been taken. Red team wins. \n");
        to_all_clients(clients, total_players, "All white pieces have been taken. Red team wins. \n");

        is_ongoing = 0;
      }
      if (num_red == 0) {
        printf("All red pieces have been taken. White team wins. \n");
        to_all_clients(clients, total_players, "All red pieces have been taken. White team wins. \n");

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
     //write(clients[i].client_socket, "0", sizeof("0"));
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
