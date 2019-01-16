#include <stdio.h>

int voting (int votes[], int length) {
  int i, j, k;
  int mode;
  int max = 0;
  int b[20] = {0};
  int c = 1;
  for (i = 0; i < length - 1; i ++) {
    mode = 0;
    for (j = i + 1; j < length; j++) {
      if (votes[i] == votes[j])
        mode++;
    }
    if ((mode > max) && (mode != 0)) {
      k = 0;
      max = mode;
      b[k] = votes[i];
      k++;
    }
    else if (mode == max) {
        b[k] = votes[i];
        k++;
    }
  }
  for (i = 0; i < length; i++){
    if (votes[i] == b[i])
        c++;
    }
  if (c == length){
      printf("\nThere is no mode");
      return -1;
  }
  else{
    for (i = 0; i < k; i++){
      if (b[i] != 0)
        mode = b[i];
    }
  }
  printf("\n");
  return mode;
}

int main() {
  int test[] = {1, 2, 4, 3, 1, 3, 2};
  int len = sizeof(test)/sizeof(test[0]);
  printf("%d\n", voting(test, len));
}
