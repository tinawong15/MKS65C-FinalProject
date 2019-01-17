#include <stdio.h>

int voting (int votes[], int length) {
  int maxValue = 0, maxCount = 0, i, j;

   for (i = 0; i < length; ++i) {
      int count = 0;

      for (j = 0; j < length; ++j) {
         if (votes[j] == votes[i])
         ++count;
      }

      if (count > maxCount) {
         maxCount = count;
         maxValue = votes[i];
      }

   }
   if (length != 1 && maxCount == 1) {
     maxValue = -1;
   }

   return maxValue;

}
/**
int main() {
  int test[] = {17,19};
  int len = sizeof(test)/sizeof(test[0]);
  //printf("%d\n", len);
  printf("%d\n", voting(test, len));
}
**/
