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
  printf("\n");
  int i;
  int row_length = 0;
  for(i = 0; i < 64; i++) {
    if(row_length == 7) {
      printf("%c\n", board[i]);
      row_length = 0;
      // printf("Row length now: %d\n", row_length);
    }
    else {
      printf("%c", board[i]);
      row_length++;
    }
  }
  printf("\n");
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

  printf("Start Checkers Game: \n");
  display(board);

  char piece[256];
  char move[4];
  int amount_of_moves = 0;
  int turn = 1; // 1 if it is white team's turn, 2 if it is red team's turn
  while(is_ongoing) {
    if(turn == 1) {
      printf("White checkers turn! Select a piece to move: \n");
      fgets(piece, 256, stdin);
      piece[strlen(piece)-1] = '\0';
      printf("Select where to move it: \n");
      fgets(move, 4, stdin);
      move[strlen(move)-1] = '\0';
      turn = 2;
    }
    else {
      printf("Red checkers turn! Select a piece to move: \n");
      fgets(piece, 256, stdin);
      piece[strlen(piece)-1] = '\0';
      printf("Select where to move it: \n");
      fgets(move, 4, stdin);
      move[strlen(move)-1] = '\0';
      turn = 1;
    }
    amount_of_moves++;
    if(amount_of_moves == 50) {
      printf("50 moves have been reached. Game is a draw.\n");
      is_ongoing = 0;
    }
    is_ongoing = 0; // TODO: remove when move/jump fxns are implemented, here just to end the while loop
  }
  return 0;
}
