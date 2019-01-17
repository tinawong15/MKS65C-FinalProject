#include "networking.h"
#include "game.h"
#include "voting.c"

void to_all_clients(struct client clients[], int num_players, char* msg);
void to_white_clients(struct client clients[], int num_players, char* msg);
void to_red_clients(struct client clients[], int num_players, char* msg);

struct client player;
struct client clients[MAX];

char board[65];
int MAX_P;
int total_players, joined;
int num_white, num_red;
int amount_of_moves, turn, is_ongoing;

char msg[BUFFER_SIZE];
char user_piece[BUFFER_SIZE], user_move[BUFFER_SIZE];
char buffer[4];
char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE], buffer3[BUFFER_SIZE], buffer4[BUFFER_SIZE];

char total[2];
char start[3];

char selected_piece;
int selected_piece_index, move_index, most_voted;

int listen_socket;
int i;
static void sighandler(int signo){
  if (signo == SIGINT){
    printf("\n Closing pipes...\n");
    to_all_clients(clients, total_players, "exit");
    to_all_clients(clients, total_players, "exit");
    exit(0);
  }
}
  // ====================================================================
void setup() {
  signal(SIGINT, sighandler);
  listen_socket = server_setup();

  printf("How many players would you like to join the game? ");
  char * max_players = calloc(sizeof(char), 100);
  fgets(max_players, 100, stdin);

  while (atoi(max_players) > MAX){
    printf("That's way too many bro. Try again: \n");
    fgets(max_players, 100, stdin);
  }
  while (atoi(max_players) < 2){
    printf("You can't play by yourself... Try again: \n");
    fgets(max_players, 100, stdin);
  }
  MAX_P = atoi(max_players);

  total_players = 0;
  joined = 0;
  while(1){
    while (total_players < MAX_P) {
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
    }
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
            strcpy(msg, "LOL. Try again :) \n");
            write(clients[i].client_socket, msg, sizeof(msg));

        }
        /**
        else {
          strcpy(msg, "Player ");
          strcat(msg, clients[i].name);
          strcat(msg, " has left.\n");
          to_all_clients(clients, total_players, msg);
          write(clients[i].client_socket, "exit", sizeof("exit"));
        }
        **/
      }

      if (joined == total_players){
        break;
      }
    }

  }

  to_all_clients(clients, total_players, "\n\n>><<>><<>><<>><<>>Welcome to Checkers!<<>><<>><<>><<>><<\n\n");


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
    to_all_clients(clients, total_players, "exit");
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
int* recieve_votes(){
  int j = 0;
  int* result = calloc(sizeof(int), total_players / 2);
  for(i = 0; i < total_players; i++) {
    if(clients[i].selected != -1) {
      //printf("%d\n", clients[i].selected);
      result[j] = clients[i].selected;
      clients[i].selected = -1;
      j++;
    }
  }
  // for (j = 0; j < total_players / 2; j++)
  //   printf("%d\n", result[j]);
  return result;
}

void ask_w_pieces(int option) {
  if (option == 1) {
    for(i = 0; i < total_players; i+= 2) {
    //printf("%d\n", i);
    write(clients[i].client_socket, "3", sizeof("3"));
    read(clients[i].client_socket, buffer2, sizeof(buffer2));
    strcpy(user_piece, buffer2);
    printf("%s\n", user_piece);
    clients[i].selected = get_piece_position(user_piece, board);
    printf("%d\n", clients[i].selected);
    }
  }
  else {
    for(i = 0; i < total_players; i+= 2) {
      printf("Select where to move it [row][column]: \n");
      //fgets(user_move, 4, stdin);
      write(clients[i].client_socket, "4", sizeof("4"));
      read(clients[i].client_socket, buffer2, sizeof(buffer2));
      printf(" user input:%s\n", buffer2);
      strcpy(user_move, buffer2);
      printf("user input:%s\n", user_move);
      clients[i].selected = get_piece_position(user_move, board);
      printf("%d\n", clients[i].selected);
    }
  }
}

void ask_r_pieces(int option){
  if (option == 1) {
    for(i = 1; i < total_players; i+= 2) {
      //printf("%d\n", i);
      write(clients[i].client_socket, "3", sizeof("3"));
      read(clients[i].client_socket, buffer2, sizeof(buffer2));
      strcpy(user_piece, buffer2);
      printf("%s\n", user_piece);
      clients[i].selected = get_piece_position(user_piece, board);
      printf("%d\n", clients[i].selected);
    }
  }
  else {
    for(i = 1; i < total_players; i+= 2) {
      printf("Select where to move it [row][column]: \n");
      //fgets(user_move, 4, stdin);
      write(clients[i].client_socket, "4", sizeof("4"));
      read(clients[i].client_socket, buffer2, sizeof(buffer2));
      printf(" user input:%s\n", buffer2);
      strcpy(user_move, buffer2);
      printf("user input:%s\n", user_move);
      clients[i].selected = get_piece_position(user_move, board);
      printf("%d\n", clients[i].selected);
    }
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

  to_all_clients(clients, total_players, "\n\n<*><*><*><*><*><*>The Checkers game is starting.<*><*><*><*><*><*>\n\n");
  //display(board);
  to_white_clients(clients, total_players, "\nYou are on the white team ['x'].\n");
  to_red_clients(clients, total_players, "\nYou are on the red team ['o'].\n");

  while(is_ongoing) {
    if(turn == 1) {
      to_all_clients(clients, total_players, "+-+-+-+-+-+-+-+-+-+-+-+-+-+White's turn+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");

      while(1) {

          ask_w_pieces(1);
          most_voted = voting(recieve_votes(), total_players / 2);
          //printf("%d\n", most_voted);
          //printf(" recieved votes: %d\n", most_voted);
          while (most_voted == -1) {
            ask_w_pieces(1);
            most_voted = voting(recieve_votes(), total_players / 2);
            to_white_clients(clients, total_players, "Voting is a tie. Please vote again.\n");
          }
          //should display piece with most votes

          selected_piece_index = most_voted;
          //printf("%d\n", selected_piece_index);
          selected_piece = board[selected_piece_index];
          //printf("%c\n", selected_piece);
          //printf("%d\n",selected_piece == 'x');
          if(selected_piece == 'x') {

            ask_w_pieces(2);
            most_voted = voting(recieve_votes(), total_players / 2);
            //printf("most voted move: %d\n", most_voted);

            while (most_voted == -1) {
              ask_w_pieces(2);
              most_voted = voting(recieve_votes(), total_players / 2);
              to_white_clients(clients, total_players, "Voting is a tie. Please vote again.\n");
            }

            move_index = most_voted;
            //printf("move index: %d\n", move_index);

            if(is_viable_move(user_piece, move_index, board) == 1) {
            // moved diagonally
              if (move_index > 55 )
                board[move_index] = 'X';
              else
                board[move_index] = selected_piece;
              board[selected_piece_index] = '-';

              break;
            }
            else if(is_viable_move(user_piece, move_index, board) == 2) {
            // then the piece jumped right
              //selected_piece_index = get_piece_position(user_piece, board);
              board[selected_piece_index+7] = '-';
              if (move_index > 55 )
                board[move_index] = 'X';
              else
                board[move_index] = selected_piece;

              board[selected_piece_index] = '-';
              num_red--;
              break;
            }
            else if(is_viable_move(user_piece, move_index, board) == 3) {
              // then the piece jumped left
              board[selected_piece_index+9] = '-';
              if (move_index > 55 )
                board[move_index] = 'X';
              else
                board[move_index] = selected_piece;
              board[selected_piece_index] = '-';
              num_red--;
              break;
            }
            else {
              strcpy(buffer4, "You cannot move here. Try again.\n");
              // printf("%s\n", buffer4);
              to_white_clients(clients, total_players, buffer4);
            }
          }
          else if(selected_piece == 'X') {
            ask_w_pieces(2);
            most_voted = voting(recieve_votes(), total_players / 2);
            //printf("%d\n", most_voted);
            //printf(" recieved votes: %d\n", most_voted);
            while (most_voted == -1) {
              ask_w_pieces(2);\
              most_voted = voting(recieve_votes(), total_players / 2);
              to_white_clients(clients, total_players, "Voting is a tie. Please vote again.\n");
            }

            move_index = most_voted;
            //printf("%d\n", move_index);

            if(is_viable_move(user_piece, move_index, board) == 1) {
            // moved diagonally
              board[move_index] = selected_piece;
              board[selected_piece_index] = '-';

              break;
            }
            else if(is_viable_move(user_piece, move_index, board) == 2) {
            // then the piece jumped right
              //selected_piece_index = get_piece_position(user_piece, board);
              board[selected_piece_index+7] = '-';
              board[move_index] = selected_piece;

              board[selected_piece_index] = '-';
              num_red--;
              break;
            }
            else if(is_viable_move(user_piece, move_index, board) == 3) {
              // then the piece jumped left
              //selected_piece_index = get_piece_position(user_piece, board);
              board[selected_piece_index+9] = '-';
              board[move_index] = selected_piece;
              board[selected_piece_index] = '-';
              num_red--;
              break;
            }
            else if(is_viable_move(user_piece, move_index, board) == 4) {
              // then the piece jumped backwards
              //selected_piece_index = get_piece_position(user_piece, board);
              board[selected_piece_index-8] = '-';

              board[move_index] = selected_piece;
              board[selected_piece_index] = '-';
              num_red--;
              break;
            }
            else if(is_viable_move(user_piece, move_index, board) == 5) {
              // then the piece jumped backwards
              board[move_index] = selected_piece;
              board[selected_piece_index] = '-';

              break;
            }
            else {
              strcpy(buffer4, "You cannot move here. Try again.\n");
              // printf("%s\n", buffer4);
              to_white_clients(clients, total_players, buffer4);

            }
          }
          else {
            strcpy(buffer4, "You cannot move this piece. Try again.\n");
            to_white_clients(clients, total_players, buffer4);
          }
        }
      //}
      //display(board);
      turn = 2;
      //strcpy(buffer, board);
      to_all_clients(clients, total_players, board);
    }

    else {
      //to_red_clients(clients,total_players,
      to_all_clients(clients, total_players, "+-+-+-+-+-+-+-+-+-+-+-+-+-+Red's turn+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");

      while(1) {
          ask_r_pieces(1);
          most_voted = voting(recieve_votes(), total_players / 2);
          //printf("most voted piece : %d\n", most_voted);
          while (most_voted == -1) {
            ask_r_pieces(1);
            most_voted = voting(recieve_votes(), total_players / 2);
            to_red_clients(clients, total_players, "Voting is a tie. Please vote again.\n");
          }
          //should display piece with most votes

          selected_piece_index = most_voted;
          //printf("%d\n", selected_piece_index);
          selected_piece = board[selected_piece_index];
          //printf("%c\n", selected_piece);
          //printf("%d\n",selected_piece == 'x');

          if(selected_piece == 'o') {
            //printf("Select where to move it [row][column]: \n");
            //fgets(user_move, 4, stdin);
            //user_move[strlen(user_move)-1] = '\0';
            ask_r_pieces(2);
            most_voted = voting(recieve_votes(), total_players / 2);
            //printf("most voted move %d\n", most_voted);
            while (most_voted == -1) {
              ask_r_pieces(2);
              most_voted = voting(recieve_votes(), total_players / 2);
              to_red_clients(clients, total_players, "Voting is a tie. Please vote again.\n");
            }

            move_index = most_voted;
            //printf("move: %d\n", move_index);

            if(is_viable_move(user_piece, move_index, board) == 1) {
              // moved diagonally
              if (move_index < 8 )
                board[move_index] = 'O';
              else
                board[move_index] = selected_piece;
              board[selected_piece_index] = '-';
              break;
            }
            else if(is_viable_move(user_piece, move_index, board) == 2) {
            // then the piece jumped right
              //selected_piece_index = get_piece_position(user_piece, board);
              board[selected_piece_index-7] = '-';
              if (move_index < 8 )
                board[move_index] = 'O';
              else
                board[move_index] = selected_piece;
              board[selected_piece_index] = '-';
              num_white--;
              break;
            }
            else if(is_viable_move(user_piece, move_index, board) == 3) {
              // then the piece jumped left
              //selected_piece_index = get_piece_position(user_piece, board);
              board[selected_piece_index-9] = '-';
              if (move_index < 8 )
                board[move_index] = 'O';
              else
                board[move_index] = selected_piece;
              board[selected_piece_index] = '-';
              num_white--;
              break;
            }
            else {
              strcpy(buffer4, "You cannot move here. Try again.\n");
              to_red_clients(clients, total_players, buffer4);
            }
          }
          else if(selected_piece == 'O') {
            ask_r_pieces(2);
            most_voted = voting(recieve_votes(), total_players / 2);
            //printf("most voted move %d\n", most_voted);
            while (most_voted == -1) {
              ask_r_pieces(2);
              most_voted = voting(recieve_votes(), total_players / 2);
              to_red_clients(clients, total_players, "Voting is a tie. Please vote again.\n");
            }
            move_index = most_voted;
            //printf("move index: %d\n", move_index);
            if(is_viable_move(user_piece, move_index, board) == 1) {
              // moved diagonally
              board[move_index] = selected_piece;
              board[selected_piece_index] = '-';
              break;
            }
            else if(is_viable_move(user_piece, move_index, board) == 2) {
            // then the piece jumped right
              //selected_piece_index = get_piece_position(user_piece, board);
              board[selected_piece_index-7] = '-';

              board[move_index] = selected_piece;
              board[selected_piece_index] = '-';
              num_white--;
              break;
            }
            else if(is_viable_move(user_piece, move_index, board) == 3) {
              // then the piece jumped left
              //selected_piece_index = get_piece_position(user_piece, board);
              board[selected_piece_index-9] = '-';
              board[move_index] = selected_piece;
              board[selected_piece_index] = '-';
              num_white--;
              break;
            }
            else if(is_viable_move(user_piece, move_index, board) == 4) {
              // then the piece jumped left
              //selected_piece_index = get_piece_position(user_piece, board);
              board[selected_piece_index+8] = '-';
              board[move_index] = selected_piece;
              board[selected_piece_index] = '-';
              num_white--;
              break;
            }
            else if(is_viable_move(user_piece, move_index, board) == 5) {
              // then the piece jumped backwards
              board[move_index] = selected_piece;
              board[selected_piece_index] = '-';

              break;
            }
            else {
              strcpy(buffer4, "You cannot move here. Try again.\n");
              //write(clients[i].client_socket, buffer4, sizeof(buffer4));
              to_red_clients(clients, total_players, buffer4);
            }
          }
          else {
            strcpy(buffer4, "You cannot move this piece. Try again.\n");
            to_red_clients(clients, total_players, buffer4);
          }
        }
      //}

      //display(board);
      //trcpy(buffer, board);
      //display(buffer);

      to_all_clients(clients, total_players, board);
      turn = 1;

    }
    amount_of_moves++;
    end_check();
    display(board);
    }
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

void to_white_clients(struct client clients[], int num_players, char* msg) {
  int i;
  char buffer[BUFFER_SIZE];
  strcpy(buffer, msg);
  for (i = 0; i < num_players; i+= 2){
    write(clients[i].client_socket, buffer, sizeof(buffer));
    //write(clients[i].client_socket, "0", sizeof("0"));
  }
}

void to_red_clients(struct client clients[], int num_players, char* msg) {
  int i;
  char buffer[BUFFER_SIZE];
  strcpy(buffer, msg);
  for (i = 1; i < num_players; i+= 2){
    write(clients[i].client_socket, buffer, sizeof(buffer));
    //write(clients[i].client_socket, "0", sizeof("0"));
  }
}
