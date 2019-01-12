# include "game.h"

/**
  h   g   f   e   d   c   b   a
1 ___ x01 ___ x02 ___ x03 ___ x04
2 x05 ___ x06 ___ x07 ___ x08 ___
3 ___ x09 ___ x10 ___ x11 ___ x12
4 ___ ___ ___ ___ ___ ___ ___ ___
5 ___ ___ ___ ___ ___ ___ ___ ___
6 o01 ___ o02 ___ o03 ___ o04 ___
7 ___ o05 ___ o06 ___ o07 ___ o08
8 o09 ___ o10 ___ o11 ___ o12 ___

x is white
o is red
X/O is kinged

50 moves without a capture/kinging is a draw
**/
void display(char * board) {
  printf("\n   h  g  f  e  d  c  b  a\n1");
  int i;
  int label = 2;
  int row_length = 0;
  for(i = 0; i < 64; i++) {
    if(row_length == 7) {
      if(label < 9) {
        printf("  %c\n%d", board[i], label);
        label++;
      }
      else {
        printf("  %c\n", board[i]);
      }
      row_length = 0;
      // printf("Row length now: %d\n", row_length);
    }
    else {
      printf("  %c", board[i]);
      row_length++;
    }
  }
  printf("\n");
}
int get_piece_position(char * input, char * board) {
  int column;
  if(input[0] == '1') {
    column = 1;
  }
  else if(input[0] == '2') {
    column = 2;
  }
  else if(input[0] == '3') {
    column = 3;
  }
  else if(input[0] == '4') {
    column = 4;
  }
  else if(input[0] == '5') {
    column = 5;
  }
  else if(input[0] == '6') {
    column = 6;
  }
  else if(input[0] == '7') {
    column = 7;
  }
  else if(input[0] == '8') {
    column = 8;
  }
  int pos = column * 8 - 1;
  if(input[1] == 'h') {
    pos = pos - 7;
  }
  else if(input[1] == 'g') {
    pos = pos - 6;
  }
  else if(input[1] == 'f') {
    pos = pos - 5;
  }
  else if(input[1] == 'e') {
    pos = pos - 4;
  }
  else if(input[1] == 'd') {
    pos = pos - 3;
  }
  else if(input[1] == 'c') {
    pos = pos - 2;
  }
  else if(input[1] == 'b') {
    pos = pos - 1;
  }
  // printf("Input: %s\n Index: %d\n Value at index: %c\n", input, pos, board[pos]);
  return pos;
}

int check_opponents(int piece_loc, char * board) {
  int piece_index = piece_loc; //get_piece_position(piece, board);
  //top left
  if ((board[piece_index - 9] != '-') && (board[piece_index - 9] != board[piece_index]) &&  (board[piece_index - 18] == '-')){
    if ((piece_index - 9) % 8 == 0 ) {
      printf ("this move is on the edge");
      return 0;}
    printf("[%c],[%c] [%d]\n", board[piece_index - 9], board[piece_index], (board[piece_index - 9] != board[piece_index]));
    printf("they different1\n");
    return 1;}
  //top right
  else if ((board[piece_index - 7] != '-') && (board[piece_index - 7] != board[piece_index]) && (board[piece_index - 14] == '-')){
    if ((piece_index - 7) % 8 == 7 ) {
      printf ("this move is on the edge");
      return 0;}
    printf("[%c],[%c] [%d]\n", board[piece_index - 7], board[piece_index], (board[piece_index - 7] != board[piece_index]));
    printf("they different2\n");
    return 2;}
  //bottom left
  else if((board[piece_index + 7] != '-' )&& (board[piece_index + 7] != board[piece_index]) && (board[piece_index + 14] == '-')){
    if ((piece_index + 7) % 8 == 0 ) {
      printf ("this move is on the edge");
      return 0;}
    printf("[%c],[%c] [%d]\n", board[piece_index + 7], board[piece_index], (board[piece_index + 7] != board[piece_index]));
    printf("they different3\n");
    return 3;}
  //bottom right
  else if((board[piece_index + 9] != '-')  && (board[piece_index + 9] != board[piece_index]) && (board[piece_index + 18] == '-')){
    if ((piece_index + 9) % 8 == 7 ) {
      printf ("this move is on the edge");
      return 0;}
    printf("[%c],[%c] [%d]\n", board[piece_index + 9], board[piece_index], (board[piece_index + 9] != board[piece_index]));
    printf("they different4\n");
    return 4;}
  //no opponent
  else
    return 0;
}
int is_viable_move(char * piece, char * move, char * board) {
  int piece_index = get_piece_position(piece, board);
  int move_index = get_piece_position(move, board);
  if(board[piece_index] == 'x') { // board piece is white
    // move diagonally
    if(piece_index + 7 == move_index || piece_index + 9 == move_index) {
      return 1;
    }

    // jump
    else if (board[piece_index+7] == 'o' && piece_index + 14 == move_index && board[move_index] == '-') {
      return 2;
    }
    else if (board[piece_index+9] == 'o' && piece_index + 18 == move_index && board[move_index] == '-') {
      return 3;
    }
  }
  else { // board piece is an 'o' (red)
    if(move_index + 7 == piece_index || move_index + 9 == piece_index) {
      return 1;
    }

    // jump
    else if (board[piece_index-7] == 'x' && piece_index - 14 == move_index && board[move_index] == '-') {
      return 2;
    }
    else if (board[piece_index-9] == 'x' && piece_index - 18 == move_index && board[move_index] == '-') {
      return 3;
    }
  }
  return 0;
  }
int jump(char piece, int piece_loc, int direction, char * board) {
  //char selected_piece;
  int selected_piece_index;
  int movement[4] = {-9, -7, 7, 9};

  //if (direction == 3) {
  //selected_piece_index = get_piece_position(user_piece, board);

  board[piece_loc + movement[direction - 1]] = '-';
  board[piece_loc + movement[direction - 1] * 2] = piece;
  board[piece_loc] = '-';
  //}

  return piece_loc + movement[direction - 1] * 2;
}

int main(int argc, char const *argv[]) {
  char board[64];
  int is_ongoing = 1;

  // initialize checkers board
  int pos;
  for(pos = 0; pos < 24; pos++) {
    if (pos < 8 || pos >= 16) {
      if(pos % 2 == 1) {
        board[pos] = 'x';
      }
      else {
        board[pos] = '-';
      }
    }
    if (pos >= 8 && pos < 16) {
      if(pos % 2 == 0) {
        board[pos] = 'x';
      }
      else {
        board[pos] = '-';
      }
    }
  }
  for(pos = 24; pos < 40; pos++) {
    board[pos] = '-';
  }
  for(pos = 40; pos < 64; pos++) {
    if(pos < 48 || pos >= 56) {
      if(pos % 2 == 0) {
        board[pos] = 'o';
      }
      else {
        board[pos] = '-';
      }
    }
    if(pos >= 48 && pos < 56) {
      if(pos % 2 == 1) {
        board[pos] = 'o';
      }
      else {
        board[pos] = '-';
      }
    }
  }

  char start[3];

  char user_piece[4];
  char user_move[4];
  char selected_piece;
  int selected_piece_index;
  int amount_of_moves = 0;
  int num_white = 12;
  int num_red = 12;
  int turn = 1; // 1 if it is white team's turn, 2 if it is red team's turn
  printf("Do you want to start playing? [y/n]\n");
  fgets(start, 3, stdin);
  start[strlen(start)-1] = '\0';
  if(strcmp(start, "y") == 0) {
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
              if (!x){
                printf("No more jumps available");
                turn = 1;
                break;
              }

            }
          }
        }
        printf("Ended search for jumps\n");

        if (!x) {
          while(1) {
            printf("White checkers turn! There are no jumps available, so select a piece to move [row][column]: \n");
            fgets(user_piece, 4, stdin);
            user_piece[strlen(user_piece)-1] = '\0';
            selected_piece = board[get_piece_position(user_piece, board)];
            if(selected_piece == 'x') {
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
              if (!x){
                printf("No more jumps available");
                turn = 1;
                break;
              }

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
  }
  else {
    exit(0);
  }

  return 0;
}
