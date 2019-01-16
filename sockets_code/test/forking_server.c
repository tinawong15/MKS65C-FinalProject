#include "networking.h"
#include "game.h"



int total_players;
struct client player;
struct client clients[MAX];
int amount_of_moves;
char msg[BUFFER_SIZE];
char total[2];
int joined;
int listen_socket;
int i;
char buffer1[BUFFER_SIZE];
int num_white;
int num_red;
void to_all_clients(struct client clients[], int num_players, char* msg);
int is_ongoing;
char start[3];
char user_piece[BUFFER_SIZE];
char user_move[BUFFER_SIZE];
char selected_piece;
int selected_piece_index;
char buffer[BUFFER_SIZE];
char buffer2[BUFFER_SIZE];
char buffer3[BUFFER_SIZE];
char buffer4[BUFFER_SIZE];
int turn;
char board[65];
static void sighandler(int signo){
  if (signo == SIGINT){
    printf("Closing pipes...\n");
    if(unlink("to_client") != -1){
      printf("Pipe to_client deleted.\n" );
    }
    if(unlink("to_parent") != -1){
      printf("Pipe to_parent deleted.\n" );
    }
    to_all_clients(clients, total_players, "exit");
    exit(0);
  }
}

void setup() {
  signal(SIGINT, sighandler);
  listen_socket = server_setup();

  total_players = 0;
  joined = 0;
  while (total_players < MAX) {
    printf("Waiting for players to connect...\n");
    int client_socket = server_connect(listen_socket);

    strcpy(buffer1, "[server] Please type your name.\n");
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
      strcat(msg, " players have joined the game. Continue? (y/n)?\n");
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
          strcat(msg, " has left.\n");
          to_all_clients(clients, total_players, msg);
          write(clients[i].client_socket, "exit", sizeof("exit"));
        }
      }

      if (joined == total_players){
        break;
      }
    }
  }

  to_all_clients(clients, total_players, "\nWelcome to Checkers!\n");

  // ====================================================================


  // printf("%s\n", board);
  for(i = 0; i < total_players; i++) {
    if(i % 2 == 0) {
      clients[i].team = 1;
    }
    else {
      clients[i].team = 2;
    }
  }
  for(i = 0; i < total_players; i++) {
    clients[i].selected = -1;
  }
}
void end_check() {
  if(amount_of_moves == 50) {
    //strcpy(msg, "50 moves have been reached. Game is a draw.\n");
    to_all_clients(clients, total_players, "50 moves have been reached. Game is a draw.\n");
    write(clients[0].client_socket, "exit", sizeof("exit"));
    write(clients[1].client_socket, "exit", sizeof("exit"));
    is_ongoing = 0;
  }
  if (num_white == 0) {
    printf("All white pieces have been taken. Red team wins. \n");
    to_all_clients(clients, total_players, "All white pieces have been taken. Red team wins. \n");
    //write(clients[0].client_socket, "exit", sizeof("exit"));
    //write(clients[1].client_socket, "exit", sizeof("exit"));
    to_all_clients(clients, total_players, "exit");
    is_ongoing = 0;
  }
  if (num_red == 0) {
    printf("All red pieces have been taken. White team wins. \n");
    to_all_clients(clients, total_players, "All red pieces have been taken. White team wins. \n");
    // write(clients[0].client_socket, "exit", sizeof("exit"));
    // write(clients[1].client_socket, "exit", sizeof("exit"));
    to_all_clients(clients, total_players, "exit");
    is_ongoing = 0;
  }
}
void checkomoves(){
}
int* recieve_votes(){
  int j = 0;
  int* result = calloc(sizeof(int), total_players / 2);
  for(i = 0; i < total_players; i++) {
    if(clients[i].selected != -1) {
      result[j] = clients[i].selected;
      clients[i].selected = -1;
    }

  }
  return result;
}

void ask_w_pieces() {
  for(i = 0; i < total_players; i+= 2) {
    write(clients[i].client_socket, "3", sizeof("3"));
    read(clients[i].client_socket, buffer2, sizeof(buffer2));
    strcpy(user_piece, buffer2);
    clients[i].selected = board[get_piece_position(user_piece, board)];
  }
}
int main() {
  setup();

  strcpy(board, init_board());

  is_ongoing = 1;

  amount_of_moves = 0;
  num_white = 12;
  num_red = 12;
  turn = 1;

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

      //for(i = 0; i < total_players; i+= 2) {
        while(1) {
          //printf("White checkers turn! There are no jumps available, so select a piece to move [row][column]: \n");
          //write(clients[1].client_socket, "10", sizeof("10"));
          //strcpy(msg, "White checkers turn! There are no jumps available, so select a piece to move [row][column]: \n");
          //printf("%s\n", msg);

          /**
          write(clients[i].client_socket, "3", sizeof("3"));
          read(clients[i].client_socket, buffer2, sizeof(buffer2));
          printf(" user input:%s\n", buffer2);
          strcpy(user_piece, buffer2);
          //fgets(user_piece, 4, stdin);
          selected_piece = board[get_piece_position(user_piece, board)];
          printf("%c\n", selected_piece);
          **/
          ask_w_pieces();
          if(selected_piece == 'x') {
            printf("Select where to move it [row][column]: \n");
            //fgets(user_move, 4, stdin);
            write(clients[i].client_socket, "4", sizeof("4"));
            printf("start reading\n");
            read(clients[i].client_socket, buffer2, sizeof(buffer2));
            printf("done reading\n");
            printf(" user input:%s\n", buffer2);
            strcpy(user_move, buffer2);
            printf("user input:%s\n", user_move);
            if(is_viable_move(user_piece, user_move, board) == 1) {
            // moved diagonally
              if (get_piece_position(user_move, board) >56 )
                board[get_piece_position(user_move, board)] = 'X';
              else
                board[get_piece_position(user_move, board)] = selected_piece;
              board[get_piece_position(user_piece, board)] = '-';

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
              write(clients[i].client_socket, buffer4, sizeof(buffer4));
            }
          }
          else if(selected_piece == 'X') {
            printf("Select where to move it [row][column]: \n");
            //fgets(user_move, 4, stdin);
            write(clients[i].client_socket, "4", sizeof("4"));
            printf("start reading\n");
            read(clients[i].client_socket, buffer2, sizeof(buffer2));
            printf("done reading\n");
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
              board[get_piece_position(user_piece, board)] = '-';

              break;
            }
            else {
              strcpy(buffer4, "You cannot move here. Try again.\n");
              // printf("%s\n", buffer4);
              write(clients[i].client_socket, buffer4, sizeof(buffer4));
            }
          }
          else {
            strcpy(buffer4, "You cannot move this piece. Try again.\n");
            write(clients[i].client_socket, buffer4, sizeof(buffer4));
          }
        }
      //}
    display(board);
    turn = 2;
    strcpy(buffer3, board);
    write(clients[0].client_socket, buffer3, sizeof(buffer3));
  }

    else {
      to_all_clients(clients, total_players, "Red's turn.\n");
      for(i = 1; i < total_players; i+= 2) {
        while(1) {
          write(clients[i].client_socket, "3", sizeof("3"));
          read(clients[i].client_socket, buffer2, sizeof(buffer2));
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
            write(clients[i].client_socket, "4", sizeof("4"));
            printf("start reading\n");
            read(clients[i].client_socket, buffer2, sizeof(buffer2));            printf("done reading\n");
            printf(" user input:%s\n", buffer2);
            strcpy(user_move, buffer2);
            printf("user input:%s\n", user_move);
            if(is_viable_move(user_piece, user_move, board) == 1) {
              // moved diagonally
              if (get_piece_position(user_move, board) < 8 )
                board[get_piece_position(user_move, board)] = 'O';
              else
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
              write(clients[i].client_socket, buffer4, sizeof(buffer4));
            }
          }
          else if(selected_piece == 'O') {
            //printf("Select where to move it [row][column]: \n");
            //fgets(user_move, 4, stdin);
            //user_move[strlen(user_move)-1] = '\0';
            write(clients[i].client_socket, "4", sizeof("4"));
            // printf("start reading\n");
            read(clients[i].client_socket, buffer2, sizeof(buffer2));            printf("done reading\n");
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
              board[get_piece_position(user_piece, board)] = '-';

              break;
            }
            else {
              strcpy(buffer4, "You cannot move here. Try again.\n");
              write(clients[i].client_socket, buffer4, sizeof(buffer4));
            }
          }
          else {
            strcpy(buffer4, "You cannot move this piece. Try again.\n");
            write(clients[i].client_socket, buffer4, sizeof(buffer4));
          }
        }
      }

        display(board);
        turn = 1;
        strcpy(buffer3, board);
        display(buffer3);
        write(clients[i].client_socket, buffer3, sizeof(buffer3));

    }
    amount_of_moves++;
    end_check();
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
