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

  // initialize board
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
  return 0;
}
